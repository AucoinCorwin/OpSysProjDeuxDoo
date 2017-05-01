#define _POSIX_SOURCE
#define _SVID_SOURCE

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <math.h>

#include "msg.h"
#include "process.h"

// Darien Keyack (661190088) and Corwin Aucoin (661178786)

struct Partition {
  int x;
  int y;
  int size;
}

int next_fit(int part_num, int** partitions, char** memory, struct Process proc) {
  int zz = 0;
  int tick = 0;
  for (zz = 0; zz < part_num; zz++) {
    if (partitions[zz][2] >= proc.memory) {
      int i = partitions[zz][0];
      int j = partitions[zz][1];
      int count = proc.memory;
      while (count > 0) {
        tick = 1;
        memory[i][j] = proc.id;
        i++;
        if (i > 31) {
          i = 0;
          j++;
        }
        count--;
      }
      break;
    }
  }
  if (tick == 0) return -1;
  else return 0;
}

int best_fit(int part_num, int** partitions, char** memory, struct Process proc) {
  int zz = 0;
  int size, loc = 99;
  for (zz = 0; zz < part_num; zz++) {
    if (partitions[zz][2] - proc.memory < size) {
      loc = zz;
      size = partitions[zz][2] - proc.memory;
    }
  }
  if (loc == 99) return -1;  // There's no space that works
  int i = partitions[loc][0];
  int j = partitions[loc][1];
  int count = proc.memory;
  while (count > 0) {
    memory[i][j] = proc.id;
    i++;
    if (i > 31) {
      i = 0;
      j++;
    }
    count--;
  }
  return 0;
}

int worst_fit(int part_num, int** partitions, char** memory, struct Process proc) {
  int zz = 0;
  int size, loc = -1;
  for (zz = 0; zz < part_num; zz++) {
    if (partitions[zz][2] - proc.memory > size) {
      loc = zz;
      size = partitions[zz][2] - proc.memory;
    }
  }
  if (loc == -1) return -1; // There's no space that works
  int i = partitions[loc][0];
  int j = partitions[loc][1];
  int count = proc.memory;
  while (count > 0) {
    memory[i][j] = proc.id;
    i++;
    if (i > 31) {
      i = 0;
      j++;
    }
    count--;
  }
  return 0;
}

int first_fit(char** memory, struct Process proc) {
  int free = 0;
  int i, j = 0;
  for (i = 0; i < 32; i++) {
    for (j = 0; j < 8; j++) {
      if (memory[i][j] == '.') free++;
    }
  }
  if (free < proc.memory) return -1;
  for (i = 0; i < 32; i++) {
    for (j = 0; j < 8; j++) {
      if ((memory[i][j] == '.') && (free > 0)) {
        memory[i][j] = proc.id;
        free--;
      }
    }
  }
  return 0;
}

int get_parts(char **memory, int **partitions) {
  int i, j, count, parts;
  count = 0;
  parts = 0;
  for (i = 0; i < 32; i++) {
    for (j = 0; j < 8; j++) {
      if (memory[i][j] == '.') count++;
      else if (count != 0) {
        parts++;
        partitions = realloc(partitions, parts * (sizeof(int) * 3));
        partitions[parts][0] = i;
        partitions[parts][1] = j;
        partitions[parts][2] = count;
        count = 0;
      }
    }
  }
  if (count == 256) {
    parts++;
    partitions = realloc(partitions, parts * (sizeof(int) * 3));
    partitions[parts][0] = 0;
    partitions[parts][1] = 0;
    partitions[parts][2] = count;
  }
  return parts;
}

int main(int argc, char * argv[]) {
  // Open input file
  if (argc < 2) msg_error("argc < 2");
  FILE *input = fopen(argv[1], "r");
  if (input == NULL) msg_error("failed to open input file");
  
  // Read number of processes from input file
  char line[256];
  fgets(line, 256, input);
  if (line == NULL) msg_error("input file too short");
  int num_proc = atoi(line);
  if ((num_proc < 1) || (num_proc > 26)) msg_error("invalid number of processes");
  
  // Read in raw processes from input file
  char** proc_raw = (char**) calloc(num_proc, sizeof(char*));
  int i = 0;
  while ((fgets(line, 256, input) != NULL) && (i < num_proc)) {
      if (line[0] != ' ' && line[0] != '#') {
          proc_raw[i] = malloc(256 * sizeof(char));
          strncpy(proc_raw[i], line, 255);
          i++;
      }
  }
  if ((i < num_proc) || (fgets(line, 256, input) != NULL)) msg_error("invalid number of processes");
  fclose(input);
  
  // Create processes from raw data
  struct Process* proc_array = (struct Process*) malloc(num_proc * sizeof(struct Process));
  int j;
  for (i = 0; i < num_proc; i++) {
      proc_array[i].id = proc_raw[i][0];
      proc_array[i].memory = get_seg(2, 256, proc_raw[i], ' ');
      j = 2 + 1 + int_len(proc_array[i].memory);
      proc_array[i].list_size = 0;
      proc_array[i].arrive_times = (int*) calloc(5, sizeof(int));
      proc_array[i].run_times = (int*) calloc(5, sizeof(int));
      while ((j < 256) && (proc_raw[i][j] != '\n') && (proc_raw[i][j] != '\0') && (proc_array[i].list_size < 5)) {
        proc_array[i].arrive_times[proc_array[i].list_size] = get_seg(j, 256, proc_raw[i], '/');
        j += 1 + int_len(proc_array[i].arrive_times[proc_array[i].list_size]);
        proc_array[i].run_times[proc_array[i].list_size] = get_seg(j, 256, proc_raw[i], ' ');
        j += 1 + int_len(proc_array[i].run_times[proc_array[i].list_size]);
        proc_array[i].list_size += 1;
      }
      free(proc_raw[i]);
  }
  free(proc_raw);
  
  // Initialize memory
  char** memory = (char**) calloc(32, sizeof(char*));
  for (i = 0; i < 32; i++) {
    memory[i] = malloc(8 * sizeof(char));
    for (j = 0; j < 8; j++) memory[i][j] = '.';
  }
  
  int** partitions = malloc(0);
  // Contiguous -- Next-Fit
  int t = 0;
  msg_sim_start(t, "Contiguous -- Next-Fit");
  while (t < 20000) {
    for (i = 0; i < num_proc; i++) {
      for (j = proc_array[i].list_size; j > 0; j--) {
        if (proc_array[i].arrive_times[j] == t) {
          //msg_arrive(t, proc_array[i].id, proc_array[i].memory);
          int num_parts = get_parts(memory, partitions);
          int result = next_fit(num_parts, partitions, memory, proc_array[i]);
          if (result > -1) {
            // TBA: Check if we have space if we defrag
            // If Yes: Defrag
            // If No: ignore
            //msg_skip(t, proc_array[i].id);
          }
          else {
            // TBA: place
             //msg_place(t, proc_array[i].id, memory);
          }
        }
        if (proc_array[i].arrive_times[j] + proc_array[i].run_times[j] == t) {
          // TBA: Handle removal
          //msg_remove(t, proc_array[i].id, memory);
        }
      }
    }
    t++;
  }
  msg_sim_end(t, "Contiguous -- Next-Fit");
  
  // Contiguous -- Best-Fit
  for (i = 0; i < 8; i++) for (j = 0; j < 32; j++) memory[i][j] = '.';
  t = 0;
  printf("\n");
  msg_sim_start(t, "Contiguous -- Best-Fit");
  // TBA: actual stuff
  msg_sim_end(t, "Contiguous -- Best-Fit");
  
  // Non-contiguous
  for (i = 0; i < 8; i++) for (j = 0; j < 32; j++) memory[i][j] = '.';
  t = 0;
  printf("\n");
  msg_sim_start(t, "Non-contiguous");
  // TBA: actual stuff
  msg_sim_end(t, "Non-contiguous");
  
  // Free & exit
  free(proc_array);
  // free memory
  return EXIT_SUCCESS;
}
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

#include "process.h"

// Darien Keyack (661190088) and Corwin Aucoin (661178786)

int next_fit(int part_num, int** partitions, char** memory, struct Process proc) {
  int zz = 0;
  for(zz = 0; zz < part_num; zz++) {
    if(partitions[zz][2] >= proc.memory) {
   
    }
  }
}

int get_parts(char **memory, int **partitions) {
  int i, j, count, parts;
  count = 0;
  parts = 0;
  for(i = 0; i < 32; i++){
    for(j = 0; j < 8; j++){
      if(memory[i][j] == '.')
        count++;
      else if(count != 0){
        parts++;
        partitions = realloc(partitions, parts * (sizeof(int) * 3));
        partitions[parts][0] = i;
        partitions[parts][1] = j;
        partitions[parts][2] = count;
        count = 0;
      }
    }
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
  fgets(line, sizeof(line), input);
  if (line == NULL) msg_error("input file too short");
  int num_proc = atoi(line);
  if (num_proc < 1 || num_proc > 26) msg_error("invalid number of processes");
  
  // Read in raw processes from input file
  char** proc_raw = (char**) calloc(num_proc, 256);
  int i = 0;
  while (fgets(line, sizeof(line), input) && i < num_proc) {
      if (line[0] != ' ' && line[0] != '#') {
          proc_raw[i] = line;
          i++;
      }
  }
  fclose(input);
  if (i < num_proc || fgets(line, sizeof(line), input) != NULL) msg_error("invalid number of processes");
  
  // Create processes from raw data
  struct Process* proc_array = (struct Process*) calloc(num_proc, sizeof(struct Process));
  int j;
  int init;
  for (i = 0; i < num_proc; i++) {
      proc_array[i].id = proc_raw[i][0];
      proc_array[i].memory = get_seg(2, 256, proc_raw[i], ' ');
      j = 2 + 1 + int_len(proc_array[i].memory);
      /*proc_array[i].t_arr = get_seg(j, 256, proc_raw[i], '/');
      j += 1 + int_len(proc_array[i].t_arr);
      proc_array[i].t_run = get_seg(j, 256, proc_raw[i], ' ');
      j += 1 + int_len(proc_array[i].t_run);*/ // t_arr and t_run are ints?
  }
    
  char memory[32][8];
  return EXIT_SUCCESS;
}
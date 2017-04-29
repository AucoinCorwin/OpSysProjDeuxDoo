
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "p2-process.h"

// Darien Keyack (661190088) and Corwin Aucoin (661178786)

// Get segment (up to forbidden char) of provided msg
int get_seg(int init, int max, char* msg, char forbidden) {
    int i;
    char line[256];
    for (i = init; i < max; i++) {
        if (msg[i] == forbidden) break;
        else line[i - init] = msg[i];
    }
    line[i - init + 1] = '\0';
    return atoi(line);
}

// Find "str len" of int; only works for positive #s
int int_len(int i) {
    return (i == 0 ? 1 : (int) (log10(i) + 1));
}

// Report error & quit
void msg_error(char *msg) {
    fprintf(stderr, "ERROR: <%s>\n", msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
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
        proc_array[i].mem_frames = get_seg(2, 256, proc_raw[i], ' ');
        j = 2 + 1 + int_len(proc_array[i].mem_frames);
        proc_array[i].t_arr = get_seg(j, 256, proc_raw[i], '/');
        j += 1 + int_len(proc_array[i].t_arr);
        proc_array[i].t_run = get_seg(j, 256, proc_raw[i], ' ');
        j += 1 + int_len(proc_array[i].t_run);
    }
    
   exit(EXIT_SUCCESS);
}
struct Process {
    char id;
    int memory;
    int* arrive_times;
    int* run_times; // This and arrive_times should be the same, so only need one variable to track their sizes
    int adj_arrive;
    int list_size;
    
};

// Get segment (up to forbidden char) of provided msg
int get_seg(int init, int max, char* msg, char forbidden) {
    int i;
    char line[256];
    for (i = init; i < max; i++) {
        if (msg[i] == forbidden) break;
        else line[i - init] = msg[i];
    }
    line[i - init] = '\0';
    return atoi(line);
}

// Find "str len" of int; only works for positive #s
int int_len(int i) {
    return (i == 0 ? 1 : (int) (log10(i) + 1));
}

// Open input file
FILE* open_input(int argc, char* filename) {
    if (argc < 2) msg_error("argc < 2");
    FILE* input = fopen(filename, "r");
    if (input == NULL) msg_error("failed to open input file");
    return input;
}

// Read number of processes from input file
int get_num_proc(FILE* input) {
    char line[256];
    fgets(line, 256, input);
    if (line == NULL) msg_error("input file too short");
    int num_proc = atoi(line);
    if ((num_proc < 1) || (num_proc > 26)) msg_error("invalid number of processes");
    return num_proc;
}

// Create processes
void create_proc(struct Process* proc_array, FILE* input, int num_proc) {
  // Read in raw processes from input file
  char** proc_raw = (char**) calloc(num_proc, sizeof(char*));
  int i = 0;
  char line[256];
  while ((fgets(line, 256, input) != NULL) && (i < num_proc)) {
      if (line[0] != ' ' && line[0] != '#') {
          proc_raw[i] = malloc(256 * sizeof(char));
          strncpy(proc_raw[i], line, 255);
          i++;
      }
  }
  if ((i < num_proc) || (fgets(line, 256, input) != NULL)) msg_error("invalid number of processes");
  
  // Create processes from raw data
  int j;
  for (i = 0; i < num_proc; i++) {
      proc_array[i].id = proc_raw[i][0];
      proc_array[i].memory = get_seg(2, 256, proc_raw[i], ' ');
      j = 2 + 1 + int_len(proc_array[i].memory);
      proc_array[i].list_size = 0;
      proc_array[i].arrive_times = (int*) calloc(5, sizeof(int));
      proc_array[i].run_times = (int*) calloc(5, sizeof(int));
      proc_array[i].adj_arrive = -1;
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
}
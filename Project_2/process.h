struct Process {
    char id;
    int memory;
    int* arrive_times;
    int* run_times; // This and arrive_times should be the same, so only need one variable to track their sizes
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

// Report error & quit
void msg_error(char *msg) {
    fprintf(stderr, "ERROR: <%s>\n", msg);
    exit(EXIT_FAILURE);
}
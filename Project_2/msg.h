// Report error & quit
void msg_error(char *msg) {
    fprintf(stderr, "ERROR: <%s>\n", msg);
    exit(EXIT_FAILURE);
}

// Simulation start
void msg_sim_start(int t, char* type) {
    printf("time %dms: Simulator started (%s)\n", t, type);
    fflush(stdout);
}

// Simulation end
void msg_sim_end(int t, char* type) {
    printf("time %dms: Simulator ended (%s)\n", t, type);
    fflush(stdout);
}

// Process arrival
void msg_arrive(int t, Process p) {
    printf("time %dms: Process %c arrived (requires %d frames)\n", t, p.id, p.memory);
    fflush(stdout);
}

// Visualize memory (TBA)
void msg_memory(char** mem) {
    printf("memory visualization not implemented!\n");
    fflush(stdout);
}

// Process placed in memory
void msg_place(int t, Process p, char** mem) {
    printf("time %dms: Placed process %c:\n", t, p.id);
    msg_memory(mem);
}

// Process removed from memory
void msg_remove(int t, Process p, char** mem) {
    printf("time %dms: Process %c removed:\n", t, p.id);
    msg_memory(mem);
}

// Skip placing process
void msg_skip(int t, Process p) {
    printf("time %dms: Cannot place process %c -- skipped!\n", t, p.id);
    fflush(stdout);
}
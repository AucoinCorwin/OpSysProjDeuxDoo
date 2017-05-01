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
void msg_arrive(int t, char id, int memory) {
    printf("time %dms: Process %c arrived (requires %d frames)\n", t, id, memory);
    fflush(stdout);
}

// Visualize memory
void msg_memory(char** mem) {
    printf("================================\n");
    int i;
    int j;
    for (j = 0; j < 8; j++) {
        for (i = 0; i < 32; i++) printf("%c", mem[i][j]);
        printf("\n");
    }
    printf("================================\n");
    fflush(stdout);
}

// Process placed in memory
void msg_place(int t, char id, char** mem) {
    printf("time %dms: Placed process %c:\n", t, id);
    msg_memory(mem);
}

// Process removed from memory
void msg_remove(int t, char id, char** mem) {
    printf("time %dms: Process %c removed:\n", t, id);
    msg_memory(mem);
}

// Skip placing process
void msg_skip(int t, char id) {
    printf("time %dms: Cannot place process %c -- skipped!\n", t, id);
    fflush(stdout);
}

// Start defragmentation
void msg_defrag_start(int t, char id) {
    printf("time %dms: Cannot place process %c -- starting defragmentation\n", t, id);
    fflush(stdout);
}

// Ending defragmentation
void msg_defrag_end(int t, int frames, char* list, char** mem) {
    printf("time %dms: Defragmentation complete (moved %d frames: ", t, frames);
    int i;
    for (i = 0; i < sizeof(list); i++) {
        if (isalnum(list[i]) > 0) {
            printf("%c", list[i]);
            if ((i < sizeof(list) - 1) && (isalnum(list[i + 1]) > 0)) printf(", ");
        }
        else break;
    }
    printf(")\n");
    msg_memory(mem);
}
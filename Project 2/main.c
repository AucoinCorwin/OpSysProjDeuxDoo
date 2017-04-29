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

struct Process {
    char id;
    int memory;
    int * arrive_times;
    int * run_times; //This and arrive_times should be the same, so only need one variable to track their sizes
    int list_size;
    
};

int get_parts(char **memory, int **partitions){
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

int main(int argc, char * argv[]){
  char memory[32][8];
  return EXIT_SUCCESS;
}
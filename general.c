#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "general.h"
#include <linux/limits.h>
void parse_commands(char * line, char ** com_ary) {
  char * curr = line;
  char * token;
  token = strsep(&curr, ";");
  int index = 0;
  while (token != NULL) {
    com_ary[index] = token;
    token = strsep(&curr, ";");
    index++;
  }
  com_ary[index] = NULL;
}
void parse_args(char * line, char ** arg_ary) {
  char * curr = line;
  char * token;
  token = strsep(&curr, " ");
  int index = 0;
  while (token != NULL) {
    arg_ary[index] = token;
    token = strsep(&curr, " ");
    index++;
  }
  arg_ary[index] = NULL;
}

// void print_args( char ** args ) {
//     for (int i = 0; i < 10; i++) {
//         if (args[i] == NULL) {
// 			break;
// 		}
// 		for (int j = 0; j < 10; j++) {
// 			if (args[i][j] == NULL) {
// 				break;
// 			}
// 			printf("%s ", args[i][j]);
// 		}
// 		printf("\n");
// 	}
// }

void execute_commands(char ** commands) {
  int command_num = 0;
  char *args[256];
  parse_args(commands[command_num], args);
  while (commands[command_num]) {
    if(strcmp(commands[command_num], "exit") == 0) {
      exit(0);
    }
    if (strcmp("cd", args[0]) == 0) {
        chdir(args[1]);
        command_num++;
        continue;
    }
    pid_t pid = fork();
  	if (pid == -1) {
    		perror("error forking");
        exit(1);
  	}
  	else if (pid == 0) { // child
        execvp(args[0], args);
  	}
  	else {
    		int status;
    		wait(&status);
  	}

    command_num++;
  }

}

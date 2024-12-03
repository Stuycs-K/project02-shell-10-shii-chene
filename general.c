#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "general.h"

void parse_args( char * line, char *** args ) {
    int num_commands = 0;
    char * command;
    while (command = strsep(&line, ";")) {
        int argc = 0;
        while (args[num_commands][argc++] = strsep(&command, " "));
        num_commands++;
    }
    args[num_commands] = NULL;
}

void print_args( char *** args ) {
    for (int i = 0; i < 10; i++) {
        if (args[i] == NULL) {
			break;
		}
		for (int j = 0; j < 10; j++) {
			if (args[i][j] == NULL) {
				break;
			}
			printf("%s ", args[i][j]);
		}
		printf("\n");
	}
}

void execute_args( char *** args ) {
  int command_num = 0;
  while (args[command_num]) {
    if(strcmp(args[command_num][0], "exit") == 0) {
      exit(0);
    }
    pid_t pid = fork();
  	if (pid == -1) {
    		perror("error forking");
        exit(1);
  	}
  	else if (pid == 0) { // child
    		execvp(args[command_num][0], args[command_num]);
  	}
  	else {
    		int status;
    		wait(&status);
  	}

    command_num++;
  }
}

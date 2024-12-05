#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "general.h"
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
void execute_pipe(char * left, char * right) {
  char *args[] = {"touch", "temp", "\0"};
  execvp(args[0], args);
 
  pid_t pid1 = fork();
  if (pid1 == -1) {
    perror("pipe fork");
  }
  if (pid1 == 0) {
    int temp = open("temp", O_WRONLY);
    int stdout = STDOUT_FILENO;
    int backup_stdout = dup(stdout);
    dup2(temp, stdout);
    char *args[256];
    parse_args(left, args);
    execvp(args[0], args);
    dup2(backup_stdout, stdout);
    close(temp);
    exit(1);
  }
  pid_t pid2 = fork();
  if (pid2 == -1) {
    perror("pipe fork 2");
  }
  if (pid2 == 0) {
     int temp = open("temp", O_RDONLY);
     int stdin = STDIN_FILENO;
     int backup_stdin = dup(stdin);
     dup2(stdin, temp);
     char *args[256];
    parse_args(right, args);
    execvp(args[0], args);
    dup2(backup_stdin, stdin);
    close(temp);
    exit(1);
  }
}

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
    char *pipe = strchr(commands[command_num], '|');
    if (pipe != NULL) {
      char * curr = commands[command_num];
      char * right = strsep(&curr, "|");
      printf("%s", curr);
      execute_pipe(curr, right);
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

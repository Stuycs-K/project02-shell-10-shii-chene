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
  int arg_num = 0;
  while (arg_ary[arg_num++] = strsep(&curr, " "));
}

void print_args( char ** args ) {
  int arg_num = 0;
  while(args[arg_num] != NULL) {
    printf("%s \n", args[arg_num]);
    arg_num++;
  }
  printf("\n");
}

void execute_pipe(char * left, char * right) {  
  pid_t pid1 = fork();
  if (pid1 == -1) {
    perror("pipe fork");
  }
  if (pid1 == 0) {
    int temp = open("temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
    int input = open("temp", O_RDONLY);
    int stdin = STDIN_FILENO;
    int backup_stdin = dup(stdin);
    dup2(input, stdin);
    char *args[256];
    parse_args(right, args);
    execvp(args[0], args);
    dup2(stdin, input);
    dup2(backup_stdin, stdin);
    close(input);
    exit(1);
  }
}

int total_args(char ** args) {
  int totalArgs = 0;
  int argNum = 0;
  while (args[argNum] != NULL) {
    totalArgs++;
    argNum++;
  }
  return totalArgs;
}

void execute_commands(char ** commands) {
  int command_num = 0;
  char *args[256];
  while (commands[command_num]) {
    char * command = commands[command_num];
    parse_args(command, args);
    if(strcmp(command, "exit") == 0) {
      exit(0);
    }
    if (strcmp("cd", args[0]) == 0) {
        chdir(args[1]);
        command_num++;
        continue;
    }

    int pipeIndices[10];
    int totalPipes = 0;
    int index = 0;
    while (args[index]) {
      if (strcmp(args[index], "|") == 0) {
        pipeIndices[totalPipes++] = index;
      }
      index++;
    }
    
    if (totalPipes > 0) {
      for (int section_num = 0; section_num <= totalPipes; section_num++) {
        char ** section = calloc(10, sizeof(char*));
        if (section_num == 0) {
          memcpy(section, args, pipeIndices[0] * sizeof(char*));
          section[pipeIndices[0]] = NULL;
        }
        else if (section_num == totalPipes) {
          memcpy(section, &args[pipeIndices[section_num - 1] + 1], (total_args(args) - pipeIndices[section_num] - 1) * sizeof(char*));
          section[total_args(args) - pipeIndices[section_num] - 1] = NULL;
        }
        else {
          memcpy(section, &args[pipeIndices[section_num - 1] + 1], (pipeIndices[section_num] - pipeIndices[section_num - 1] - 1) * sizeof(char*));
          section[pipeIndices[section_num] - pipeIndices[section_num - 1] - 1] = NULL;
        }
        print_args(section);
        free(section);
      }
    }
    else {
      print_args(args);
    }

    // char *pipe = strchr(command, '|');
    // if (pipe != NULL) {
    //   char * right = command;
    //   char * left = strsep(&right, "|");
    //   printf("%s, %s\n", left, right);
    //   execute_pipe(left, right);
    //   command_num++;
    //   continue;
    // }
    // pid_t pid = fork();
  	// if (pid == -1) {
    // 		perror("error forking");
    //     exit(1);
  	// }
  	// else if (pid == 0) { // child
    
    //     execvp(args[0], args);
  	// }
  	// else {
    // 		int status;
    // 		wait(&status);
  	// }

    command_num++;
  }
}

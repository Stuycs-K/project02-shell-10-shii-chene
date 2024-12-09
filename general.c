#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#include "general.h"
#include "utils.h"

void parse_commands(char * line, char ** com_ary) {
  printf("%s\n", line);
  char * curr = line;
  char * token;
  token = strsep(&curr, ";");
  int index = 0;
  while (token != NULL) {
    com_ary[index] = token;
    token = strsep(&curr, ";");
    index++;
  }
  printf("%s\n", com_ary[0]);
  com_ary[index] = NULL;
}

void parse_args(char * line, char ** arg_ary) {
  char * curr = line;
  int arg_num = 0;
  while (arg_ary[arg_num++] = strsep(&curr, " "));
}

// each section will only have one < or > at most
void handle_section(char ** section, bool isFirstSection, bool isLastSection) {
  if(strcmp(section[0], "exit") == 0) {
    exit(0);
  }
  if (strcmp(section[0], "cd") == 0) {
    chdir(section[1]);
    return;
  }

  char redirect = '\0'; // doesnt include pipe, since sections are already split on pipes
  int arg = 0;
  while (section[arg]) {
    if (strcmp(section[arg], "<") == 0 || strcmp(section[arg], ">") == 0) {
      redirect = section[arg][0];
      break;
    }
    arg++;
  }

  if (redirect == '\0') {
    if (isLastSection) {
      if (temp_exists()) {
        int temp = open("temp.txt", O_RDONLY);
        int stdin = STDIN_FILENO;
        int backup_stdin = dup(stdin);
        dup2(temp, stdin);

        pid_t pid = fork();
        if (pid == -1) {
            perror("error forking");
            exit(1);
        }
        else if (pid == 0) { // child
          execvp(section[0], section);
        }
        else {
          int status;
          wait(&status);
        }

        dup2(backup_stdin, stdin);
      }
      else {
        pid_t pid = fork();
        if (pid == -1) {
            perror("error forking");
            exit(1);
        }
        else if (pid == 0) { // child
          execvp(section[0], section);
        }
        else {
          int status;
          wait(&status);
        }
      }
    }
    else if (isFirstSection) {
      int temp = open("temp.txt", O_WRONLY | O_CREAT, 0644);
      int stdout = STDOUT_FILENO;
      int backup_stdout = dup(stdout);
      dup2(temp, stdout);

      pid_t pid = fork();
      if (pid == -1) {
          perror("error forking");
          exit(1);
      }
      else if (pid == 0) { // child
        execvp(section[0], section);
      }
      else {
        int status;
        wait(&status);
      }

      dup2(backup_stdout, stdout);
    }
    else {
      int temp = open("temp.txt", O_WRONLY | O_RDONLY);
      int stdout = STDOUT_FILENO;
      int backup_stdout = dup(stdout);
      dup2(temp, stdout);
      int stdin = STDIN_FILENO;
      int backup_stdin = dup(stdin);
      dup2(temp, stdin);
      
      pid_t pid = fork();
      if (pid == -1) {
          perror("error forking");
          exit(1);
      }
      else if (pid == 0) { // child
        execvp(section[0], section);
      }
      else {
        int status;
        wait(&status);
      }

      dup2(backup_stdin, stdin);
      dup2(backup_stdout, stdout);
    }
  }
  else if (redirect == '<') {
    char * fileName = section[total_args(section) - 1];
    section[total_args(section) - 2] = NULL; // location of the redirect
    int file = open(fileName, O_RDONLY);
    int stdin = STDIN_FILENO;
    int backup_stdin = dup(stdin);
    dup2(file, stdin);

    if (!isLastSection) {
      int temp = open("temp.txt", O_WRONLY | O_CREAT, 0644);
      int stdout = STDOUT_FILENO;
      int backup_stdout = dup(stdout);
      dup2(temp, stdout);

      pid_t pid = fork();
      if (pid == -1) {
          perror("error forking");
          exit(1);
      }
      else if (pid == 0) { // child
        execvp(section[0], section);
      }
      else {
        int status;
        wait(&status);
      }

      dup2(backup_stdout, stdout);
    }
    else {
      pid_t pid = fork();
      if (pid == -1) {
          perror("error forking");
          exit(1);
      }
      else if (pid == 0) { // child
        execvp(section[0], section);
      }
      else {
        int status;
        wait(&status);
      }
    }

    section[total_args(section) - 2] = "<"; // put the redirect back, although its not really used anywhere else
    dup2(backup_stdin, stdin);
  }
  else if (redirect == '>') {
    char * fileName = section[total_args(section) - 1];
    section[total_args(section) - 2] = NULL; // location of redirect
    int file = open(fileName, O_WRONLY | O_CREAT, 0644);
    int stdout = STDOUT_FILENO;
    int backup_stdout = dup(stdout);
    dup2(file, stdout);

    if (!isFirstSection) {
      int temp = open("temp.txt", O_WRONLY | O_CREAT, 0644);
      int stdin = STDIN_FILENO;
      int backup_stdin = dup(stdin);
      dup2(temp, stdin);

      pid_t pid = fork();
      if (pid == -1) {
          perror("error forking");
          exit(1);
      }
      else if (pid == 0) { // child
        execvp(section[0], section);
      }
      else {
        int status;
        wait(&status);
      }

      dup2(backup_stdin, stdin);
    }
    else {
      pid_t pid = fork();
      if (pid == -1) {
          perror("error forking");
          exit(1);
      }
      else if (pid == 0) { // child
        execvp(section[0], section);
      }
      else {
        int status;
        wait(&status);
      }
    }

    section[total_args(section) - 2] = ">"; // put the redirect back, although its not really used anywhere else
    dup2(backup_stdout, stdout);
  }
  else {
    perror("invalid redirect detected\n");
    exit(1);
  }
} 

void execute_commands(char ** commands) {
  int command_num = 0;
  char *args[256];
  while (commands[command_num]) {
    char * command = commands[command_num];
    parse_args(command, args);

    int pipeIndices[10];
    int totalPipes = 0;
    int index = 0;
    while (args[index]) {
      if (strcmp(args[index], "|") == 0) {
        pipeIndices[totalPipes++] = index;
      }
      index++;
    }

    pid_t pid = fork();
  	if (pid == -1) {
    		perror("error forking");
        exit(1);
  	}
  	else if (pid == 0) { // child
      for (int section_num = 0; section_num <= totalPipes; section_num++) {
        char ** section = calloc(10, sizeof(char*));
        if (section_num == 0) {
          if (totalPipes == 0) {
            handle_section(args, true, true);
          }
          else {
            memcpy(section, args, pipeIndices[0] * sizeof(char*));
            section[pipeIndices[0]] = NULL;
            handle_section(section, true, false);
          }
        }
        else if (section_num == totalPipes) {
          memcpy(section, &args[pipeIndices[section_num - 1] + 1], (total_args(args) - pipeIndices[section_num - 1] - 1) * sizeof(char*));
          section[total_args(args) - pipeIndices[section_num - 1] - 1] = NULL;
          handle_section(section, false, true);
        }
        else {
          memcpy(section, &args[pipeIndices[section_num - 1] + 1], (pipeIndices[section_num] - pipeIndices[section_num - 1] - 1) * sizeof(char*));
          section[pipeIndices[section_num] - pipeIndices[section_num - 1] - 1] = NULL;
          handle_section(section, false, false);
        }
        free(section);
      }
  	}
  	else {
      int status;
      wait(&status);
  	}

    command_num++;
  }
}

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

void handle_section(char **section, int isFirstSection, int isLastSection) {
    char redirect = '\0';// doesnt include pipe, since sections are already split on pipes
    int arg = 0;
    while (section[arg]) {
        if (strcmp(section[arg], "<") == 0 || strcmp(section[arg], ">") == 0) {
            redirect = section[arg][0];
            break;
        }
        arg++;
    }

    if (redirect == '\0') { // No redirection
        if (isLastSection) {
            if (temp_exists()) {
                int temp = open("temp.txt", O_RDONLY);
                if (temp == -1) {
                    perror("Error opening temp file for reading");
                    exit(1);
                }
                int backup_stdin = dup(STDIN_FILENO);
                dup2(temp, STDIN_FILENO);

                pid_t pid = fork();
                if (pid == -1) {
                    perror("Error forking");
                    exit(1);
                } else if (pid == 0) { // Child process
                    execvp(section[0], section);
                    printf("%s", section[1]);
                    perror("Error executing command 1");
                    exit(1);
                } else { // Parent process
                    close(temp);
                    wait(NULL);
                }

                dup2(backup_stdin, STDIN_FILENO);
                close(backup_stdin);
            } else {
                pid_t pid = fork();
                if (pid == -1) {
                    perror("Error forking");
                    exit(1);
                } else if (pid == 0) { // Child process
                    execvp(section[0], section);
                    perror("Error executing command");
                    exit(1);
                } else { // Parent process
                    wait(NULL);
                }
            }
        } else if (isFirstSection) {
            int temp = open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (temp == -1) {
                perror("Error opening temp file for writing");
                exit(1);
            }
            int backup_stdout = dup(STDOUT_FILENO);
            dup2(temp, STDOUT_FILENO);

            pid_t pid = fork();
            if (pid == -1) {
                perror("Error forking");
                exit(1);
            } else if (pid == 0) { // Child proces
                execvp(section[0], section);
                perror("")
                exit(1);
            } else { // Parent process
                close(temp);
                wait(NULL);
            }

            dup2(backup_stdout, STDOUT_FILENO);
            close(backup_stdout);
        } else { // Intermediate section
            int temp_read = open("temp.txt", O_RDONLY);
            int temp_write = open("temp.txt", O_WRONLY | O_TRUNC);
            if (temp_read == -1 || temp_write == -1) {
                perror("Error opening temp file 2nd pipe");
                exit(1);
            }

            int backup_stdin = dup(STDIN_FILENO);
            int backup_stdout = dup(STDOUT_FILENO);
            dup2(temp_read, STDIN_FILENO);
            dup2(temp_write, STDOUT_FILENO);

            pid_t pid = fork();
            if (pid == -1) {
                perror("Error forking");
                exit(1);
            } else if (pid == 0) { // Child process
                execvp(section[0], section);
                perror("Error second command");
                exit(1);
            } else { // Parent process
                close(temp_read);
                close(temp_write);
                wait(NULL);
            }

            dup2(backup_stdin, STDIN_FILENO);
            dup2(backup_stdout, STDOUT_FILENO);
            close(backup_stdin);
            close(backup_stdout);
        }
    } else if (redirect == '<') { 
        char *fileName = section[arg + 1];
        section[arg] = NULL; 
        int file = open(fileName, O_RDONLY);
        if (file == -1) {
            perror("Error opening input file");
            exit(1);
        }

        int backup_stdin = dup(STDIN_FILENO);
        dup2(file, STDIN_FILENO);

        pid_t pid = fork();
        if (pid == -1) {
            perror("Error forking");
            exit(1);
        } else if (pid == 0) { // Child process
            execvp(section[0], section);
            perror("Error executing command");
            exit(1);
        } else { // Parent process
            close(file);
            wait(NULL);
        }

        dup2(backup_stdin, STDIN_FILENO);
        close(backup_stdin);
    } else if (redirect == '>') { // Output redirection
        char *fileName = section[arg + 1];
        section[arg] = NULL; // Remove redirect symbol and file from arguments
        int file = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file == -1) {
            perror("Error opening output file");
            exit(1);
        }

        int backup_stdout = dup(STDOUT_FILENO);
        dup2(file, STDOUT_FILENO);

        pid_t pid = fork();
        if (pid == -1) {
            perror("Forking");
            exit(1);
        } else if (pid == 0) { // Child 
            execvp(section[0], section);
            perror("Execution error");
            exit(1);
        } else { // Parent 
            close(file);
            wait(NULL);
        }

        dup2(backup_stdout, STDOUT_FILENO);
        close(backup_stdout);
    } else {
        fprintf(stderr, "Invalid redirect detected\n");
        exit(1);
    }
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
  if (strcmp(command, "cd") == 0) {
    if (chdir(args[1]) == -1) {
     perror("no change directory");
    }
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
        command_num++;
      }
  	}
  	
  }


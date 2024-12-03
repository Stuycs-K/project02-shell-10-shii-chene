#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include "general.h"
#include <fcntl.h>

// char* user_input() {
//   char cwd[256];
//   getcwd(cwd, sizeof(cwd));
//   char *curr = cwd;
//   char *token;
//   while (token != NULL) {
//     token = strsep(&curr, "/");
//   }
//   char line[256];
//   printf("~/%s/", curr);
//   fgets(line, sizeof(line), stdin);
//   line[strcspn(line, "\n")] = '\0';
//   return line;

// }
void parse_commands(char * line, char ** commands) {
  char * curr = line;
  char * token;
  token = strsep(&curr, ";");
  int index = 0;
  while (token != NULL) {
    commands[index] = token;
    token = strsep(&curr, " ");
    index++;
  }
  commands[index] = NULL;
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

void run() {
  char line[256];
  char *args[256];
  char *commands[256];
  while (1) {
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
     char *curr = cwd;
    char *token;
    while (token != NULL) {
      token = strsep(&curr, "/");
    }
    printf("~/%s/", curr); // prompt
    fgets(line, sizeof(line), stdin);
    line[strcspn(line, "\n")] = '\0';
    parse_commands(line, commands);
    int index = 0;
    while (commands[index] != NULL) {
      parse_args(command[i], args);
      if (strcmp(args[0], "exit") == 0) {
        exit(0);
      }
      pid_t pid = fork();
      if (pid == -1) {
        perror("forking went wrong");
      }
      else if (pid == 0) { // child
        execvp(args[0], args);
      }
      else {
        int status;
        wait(&status);
        index++;
      }
    }
    
  }


}

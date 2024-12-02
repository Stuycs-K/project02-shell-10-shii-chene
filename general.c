#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include "general.h"
#include <fcntl.h>

void user_input() {
  char cwd[256];
  getcwd(cwd, sizeof(cwd));
  char *curr = cwd;
  char *token;
  while (token != NULL) {
    token = strsep(&curr, "/");
  }
  printf("~/%s/", curr);
  fgets()
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

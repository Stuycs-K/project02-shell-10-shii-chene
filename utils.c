#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "utils.h"

void print_args( char ** args ) {
  int arg_num = 0;
  while(args[arg_num] != NULL) {
    printf("%s ", args[arg_num]);
    arg_num++;
  }
  printf("\n");
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
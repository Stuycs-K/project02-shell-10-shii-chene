#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

#include "utils.h"

// char ** args - array of strings(arguments) to be printed
// return value void
// print_args prints the arguments in an argument array
void print_args( char ** args ) {
  int arg_num = 0;
  while(args[arg_num] != NULL) {
    printf("%s ", args[arg_num]);
    arg_num++;
  }
  printf("\n");
}

// char ** args - array of strings(arguments) 
// return value int - total number of arguments in args array
// total_arg counts the number of arguments in an argument array
int total_args(char ** args) {
  int totalArgs = 0;
  int argNum = 0;
  while (args[argNum] != NULL) {
    totalArgs++;
    argNum++;
  }
  return totalArgs;
}

// No arguments
// return type bool - True if temp.txt exists
// temp_exists() checks if "text.txt" exists and returns True if it does

bool temp_exists() {
  return access("temp.txt", F_OK) == 0;
}
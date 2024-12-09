#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

#include "prompt.h"
// No args
// return type void4
// prompt gets the user's current directory and provides a prompt for the user to input commands
void prompt() {
  char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	printf("%s $ ", cwd);
	fflush(stdout);
}

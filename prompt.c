#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

#include "prompt.h"

void prompt() {
    char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	printf("%s $ ", cwd);
	fflush(stdout);
}
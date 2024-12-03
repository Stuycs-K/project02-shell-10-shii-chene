#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

void redir_stdout(char* command) {
	char line[256];
	strcpy(line, command);
	char * curr = line;
	char * token;
	token = strsep(&curr, ">");

}


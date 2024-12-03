#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

#include "general.h"
#include "prompt.h"

int main() {
	char * args[16];
	char line[100];

	prompt();
	if(fgets(line, sizeof(line), stdin) != NULL) {
		// Remove trailing newline if present
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
		
		parse_args( line, args );
	}
	else {
		perror("error reading user input");
		exit(1);
	}
}

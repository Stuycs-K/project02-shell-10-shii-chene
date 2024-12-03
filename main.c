#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

#include "general.h"
#include "prompt.h"

int main() {
	char line[200];

	char ** args[200];
	for (int i = 0; i < 10; i++) {
		args[i] = malloc(sizeof(char) * 200);
	}

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

	execute_args(args);
}

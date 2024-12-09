#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

#include "general.h"
#include "prompt.h"
#include "utils.h"

int main() {
	char line[200];
	char * commands[200];
	while (1) {
		prompt();
		if(fgets(line, sizeof(line), stdin) != NULL) {
			// Remove trailing newline if present
	        int len = strlen(line);
	        if (len > 0 && line[len - 1] == '\n') {
	            line[len - 1] = '\0';
	        }

			parse_commands(line, commands);
		}
		else {
			perror("error reading user input\n");
			exit(1);
		}
		execute_commands(commands);
	}
}

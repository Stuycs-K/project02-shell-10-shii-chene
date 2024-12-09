#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

#include "general.h"
#include "prompt.h"
#include "utils.h"

int main() {
	char line[256];
	char * commands[256];
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
			printf("\n");
			fflush(stdout);
			exit(0);
			
		}
		execute_commands(commands);
	}
}

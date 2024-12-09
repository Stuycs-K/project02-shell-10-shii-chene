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
    char * commands[256];;

    
    while (1) {
        prompt();
        if (fgets(line, sizeof(line), stdin) == NULL) {
			if (feof(stdin)) {
                printf("\n");
                break;
			}
			else {
				perror("error reading input");
                printf("\n");
            	exit(1);
			}
        }

        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        parse_commands(line, commands);
        execute_commands(commands);
    }
    return 0;
}
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "general.h"

void parse_args( char * line, char *** args ) {
    int num_commands = 0;
    char * command;
    while (command = strsep(&line, ";")) {
        int argc = 0;
        while (args[num_commands][argc++] = strsep(&command, " "));
        num_commands++;
    }
    args[num_commands] = NULL;
}

void print_args( char *** args ) {
    for (int i = 0; i < 10; i++) {
        if (args[i] == NULL) {
			break;
		}
		for (int j = 0; j < 10; j++) {
			if (args[i][j] == NULL) {
				break;
			}
			printf("%s ", args[i][j]);
		}
		printf("\n");
	}
}

void handle_args( char *** args ) {
    
}
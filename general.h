#ifndef GENERAL_H
#define GENERAL_H
void parse_args( char * line, char ** args );
void print_args( char ** args );
void execute_commands(char ** commands, char ** args );
void parse_commands(char * line, char ** com_ary);
#endif

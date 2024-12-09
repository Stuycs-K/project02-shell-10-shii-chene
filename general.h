#ifndef GENERAL_H
#define GENERAL_H
void parse_commands(char * line, char ** commands);
void parse_args(char * line, char ** com_ary);
void execute_commands(char ** commands);
void execute_pipe(char * left, char * right);
#endif

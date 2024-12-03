#include <string.h>

#include "general.h"

void parse_args( char * line, char ** arg_ary ) {
    int argc = 0;
    while (arg_ary[argc++] = strsep(&line, " "));
}
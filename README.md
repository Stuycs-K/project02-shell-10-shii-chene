[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)
# Systems Project2 - MyShell
Ian Shii Elizabeth Chen
Team The Ian-credible and Liz-tacular 

# Features implemented:
  - General Functionality
  - Working Directory
  - Prompt
  - Exit
  - Redirection and Pipes
  - 
# void parse_commands(char * line, char ** com_ary)
char * line - the line of commands to be parsed
char ** com_ary - where the parsed commands will be stored
return type void
parse_commands splits the input (line) command line into individual commands 
using strsep and a semicolon as the delimited. The resulting commands are 
stored in the com_ary.


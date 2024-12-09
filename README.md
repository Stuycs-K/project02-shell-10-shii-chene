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
    
# void parse_commands(char * line, char ** com_ary)
- char * line - the line of commands to be parsed
- char ** com_ary - where the parsed commands will be stored
- return type void
- parse_commands splits the input (line) command line into individual commands 
using strsep and a semicolon as the delimited. The resulting commands are 
stored in the com_ary.

# void parse_args(char * line, char ** arg_ary)
- char * line - the command to be parsed 
- char ** arg_ary - where the arguments of the command will be stored
- return type void
- parse_args parses a command(line) into individual arguments and stores the into arg_ary

# void handle_section(char ** section, bool isFirstSection, bool isLastSection)
- char ** section - the array of arguments of the command section
- bool isFirstSection - indicates if this is the first section of the command
- bool isLastSection - indicated if this is the last section of the command
- return type void
- handle_section handles exiting, and changing the directory. It forks to execute programs. It also handles any redirections and pipes.

# void execute_commands(char ** commands) 
- char ** commands - an array of commands 
- return type void
- execute_commands iterates through the commands, parses its arguments, and passes it on to handle_section to be executed.

# void prompt()
- No args
- return type void4
- prompt gets the user's current directory and provides a prompt for the user to input commands

# void print_args( char ** args )
- char ** args - array of strings(arguments) to be printed
- return value void
- print_args prints the arguments in an argument array

# int total_args(char ** args)
- char ** args - array of strings(arguments) 
- return value int - total number of arguments in args array
- total_arg counts the number of arguments in an argument array

# bool temp_exists()
- No arguments
- return type bool - True if temp.txt exists
- temp_exists() checks if "text.txt" exists and returns True if it does

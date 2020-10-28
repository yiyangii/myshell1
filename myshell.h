#ifndef MYSHELL_H
#define MYSHELL_H

#define SUCCESS 0
#define FAILURE 1
#define ERR_MEMORY_ALLOC 2
#define CMD_NOT_FOUND 3
#define TOO_MANY_PARAMETERS 4
#define TOO_FEW_PARAMETERS 5

#define SHELL_PROMPT "LAB2MYSHELL"


#define MAX_BUFFER 1024
void ParseNExecuteCommands(const char *command);

#endif //MYSHELL_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include "myshell.h"
#include "implementedcommands.h"


//make command list 
//Assign each command line to function
static const implemented_command_list implemented_commands[] ={{"cd",CD},{"clr",CLR },{"dir",DIRECTORY},{"environ",ENVIRON},{"echo",ECHO},{"help",HELP},{"pause",PAUSE},{"quit",QUIT},{"path",PATH},{0,0}};
int ArgsCount(char *const *args);
//Get the num of internal command 
int numImplementedCommands(){
    return sizeof(implemented_commands) / sizeof(implemented_commands[0]);
}
//Get the number of Args using size of command length/ single command
int ArgsCount(char *const *args){
    int num_of_args;
    for (num_of_args = 0; ((num_of_args < MAX_NUM_OF_ARGUMENTS) && (args[num_of_args+1] != NULL)); num_of_args++){

    }
    return num_of_args;
}

//Make a loop through each command 
//if the command match the function then return that command 
//IF the user input does not match the command then it will return 0;
int ExecImplementedCmd(Command *cmd)
{
    for (int i=0; ((i < numImplementedCommands()) && (implemented_commands[i].cmd != NULL)); i++)
    {
        if(strcmp(cmd->tokenizedCommands[0], implemented_commands[i].cmd) == 0) {

            return (implemented_commands[i].pfunc)(cmd); 
                }
    }
    return CMD_NOT_FOUND;
}
//Add the path to the modifiedpath then setenv
int PATH (Command *cmd){
    char modifiedPath[MAX_NUM_OF_ARGUMENTS * MAX_ARG_LENGTH] = {0};
    int num_of_args = ArgsCount(cmd->tokenizedCommands);
    while (num_of_args > 0){
        strcat(modifiedPath, cmd->tokenizedCommands[num_of_args]);
        strcat(modifiedPath, ":");
        num_of_args--;
    }
    setenv("PATH", modifiedPath, 1);
    return EXIT_SUCCESS;
}

int DIRECTORY (Command *cmd){
    FILE * fp=NULL;
    int numArgs = ArgsCount(cmd->tokenizedCommands);
        //set a pointer to dir
    struct dirent *parentDir;  
        // check to see if the output can be redirected
          //if yes assign path to the taeget file 
          //check to see if user want to append of write in new file 
    if (cmd->isOutputRedirected){
        char filePath[100] = {0};
        derivefullpath(filePath, cmd->redirectedOutput);
        fp=freopen(filePath, cmd->isOutputTruncated == 1 ? "w" : "a", stdout);
    }
        
    DIR *dir;
        //if command line is dir then return with the file exist in the foloder
    if (numArgs == 0){
        dir = opendir(".");
    }
        //open the dir to user input
    else{
        dir = opendir(cmd->tokenizedCommands[numArgs]);
        }
    if (dir == NULL){
        errorMessage();
        return FAILURE;
    }
        //read dir file 
    while ((parentDir = readdir(dir)) != NULL)
        printf("%s\n", parentDir->d_name);

    closedir(dir);

    if(fp)
    {
        fclose(fp);
        freopen("/dev/tty","w",stdout);
    }

    return SUCCESS;
}

//CD command 
//chdie("..") back the the last directory
//if argument != 1 then change dir to the given directory
int CD (Command *cmd){
    int numArgs = ArgsCount(cmd->tokenizedCommands);
    if (numArgs == 0){
        chdir(".."); 
        return SUCCESS;    
        }
    int ret = chdir(cmd->tokenizedCommands[numArgs]);
    if (ret){
        
        return FAILURE;
    }
    return SUCCESS;
}

int CLR (Command *cmd){
     printf("%s", "\033[H\033[2J");
    return EXIT_SUCCESS;
}

//using __environ[]   to get the output


int ENVIRON (Command *cmd){
    FILE * fp=NULL;
    int i = 0;

    if (cmd->isOutputRedirected)
    {
        char filePath[100] = {0};
        derivefullpath(filePath, cmd->redirectedOutput);
        fp=freopen(filePath, cmd->isOutputTruncated == 1 ? "w" : "a", stdout);
    }

    while(__environ[i]) {
        printf("%s\n", __environ[i++]); // prints in form of "variable=value"
    }

    if(fp)
    {
        fclose(fp);
        freopen("/dev/tty","w",stdout);
    }
    return SUCCESS;
}
int ECHO (Command *cmd){
    int numArgs;
    FILE * fp=NULL;

    if (cmd->isOutputRedirected){
        char filepath[100] = {0};
        derivefullpath(filepath, cmd->redirectedOutput);
        fp=freopen(filepath, cmd->isOutputTruncated == 1 ? "w" : "a", stdout);
    }
        //print command 

    for (numArgs = 1; ((numArgs < MAX_NUM_OF_ARGUMENTS) && (cmd->tokenizedCommands[numArgs] != NULL)); numArgs++)
    {
        printf("%s \t",cmd->tokenizedCommands[numArgs]);
    }
    printf("\n");

    if(fp)
    {
        fclose(fp);
        freopen("/dev/tty","w",stdout);
    }
    return FAILURE;
}
//open readme file then read thrugh it 
//write to screen
int HELP (Command *cmd){
    FILE * fp=NULL;
    if (cmd->isOutputRedirected)
    {
        char filePath[100] = {0};
        derivefullpath(filePath, cmd->redirectedOutput);
        fp=freopen(filePath, cmd->isOutputTruncated == 1 ? "w" : "a", stdout);
    }

    if (fork() == 0)
    {
        char * const help[] = { "more", "readme_doc", NULL };
        if (execvp(help[0], help) == -1)
        {
            errorMessage();
            exit(EXIT_FAILURE);
        }
    }
    if(fp)
    {
        fclose(fp);
        freopen("/dev/tty","w",stdout);
    }

    return EXIT_SUCCESS;
}
int PAUSE (Command *cmd)
{
    getpass("_________________________Paused \n <ENTER> key to continue______________________________");
    return EXIT_SUCCESS;
}

int QUIT (Command *cmd)
{
        printf("%s", "_______________________Quitting Shell______________________________\n");
        sleep(1);
    exit(0);
}


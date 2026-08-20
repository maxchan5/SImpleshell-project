//header file for shell program

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 1024           // max line buffer
#define MAX_ARGS 64               // max # args
#define SEPARATORS " \t\n"        // token separators

//function prototypes for shell commands

void cd_cmd(char **args);  //changes current working directory

void clr_cmd();           // clears the screen

void dir_cmd(char**args);  // lists directory contents

void environ_cmd();       // lists all environment strings

void echo_cmd(char **args);  // prints text entered after "echo"

void help_cmd();            // displays manual

void pause_cmd();           // pauses operation until "Enter" is pressed

void run_cmd(char **args, int background, char *inputf, char *outputf, int append, char *shellpath);   // runs commands handled in child process


//simpleshell reads commands from the user/batchfile and executes
//built in commands cd, clr, dir, environ, echo, help, pause, quit

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include "simpleshell.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators

int main (int argc, char ** argv)
{
    char buf[MAX_BUFFER];                      // line buffer
    char * args[MAX_ARGS];                     // pointers to arg strings
    char **arg;                                // working pointer thru args
    char cwd[PATH_MAX];                        //stores current working directory


    FILE *input = stdin;                      //default input is standard input

    if (argc > 1) {                        //checks if a batchfile was given in the arguemnt
      input = fopen(argv[1], "r");
      if (input == NULL) {
        perror("batch file error");
        exit(1);
      }
    }

    // set environment variable "shell" to the full path
    char shellpath[PATH_MAX];
    if (realpath(argv[0], shellpath) == NULL) {   //error handling
      perror("realpath failed");
      exit(1);
    }
    setenv("shell", shellpath, 1);

    while (1) {                        //main loop, continues until end of file or quit command is used

      if (input == stdin) {            //display prompt if not read from batchfile
        getcwd(cwd, sizeof(cwd));      //get current working directory
        printf("%s ==> ", cwd);        // display prompt
      }

      //reads line of input
      if (!fgets (buf, MAX_BUFFER, input )) { // returns NULL at end of file
        break;                                // exits loop if there are no more input
      }

     // tokenize the input line into seperate arguments
      arg = args;
      *arg++ = strtok(buf,SEPARATORS);

      while ((*arg++ = strtok(NULL, SEPARATORS)));

      if (args[0] == NULL) {                //if empty, skip
        continue;
      }

      //flags and variables
      int background = 0;           //run in background
      char *inputf = NULL;          //input file
      char *outputf = NULL;         //output file
      int append = 0;               //append >>

      for (int i = 0; args[i] != NULL; i++) {

        if (strcmp(args[i], "&") == 0) {   //background execution
            background = 1;
            args[i] = NULL;              // remove & from argument list
        
        } else if (strcmp(args[i], "<") == 0) {     //input redirection
            if (args[i + 1] == NULL) {
              printf("no input file\n");
              break;
            }
            inputf = args[i + 1];
            args[i] = NULL; 
            args[i + 1] = NULL;
            i++;                              // skip filename
        
        } else if (strcmp(args[i], ">") == 0) {       //output redirection overwrite
            if (args[i + 1] == NULL) {
              printf("no output file\n");
              break;
            }
            outputf = args[i + 1];
            append = 0;
            args[i] = NULL;
            args[i + 1] = NULL;
            i++;
        
        } else if (strcmp(args[i], ">>") == 0) {      // output redirection append
            if (args[i + 1] == NULL) {
              printf("no output file\n");
              break;
            }
            outputf = args[i + 1];
            append = 1;
            args[i] = NULL;
            args[i + 1] = NULL;
            i++;
        }
      }

      if (args[0] == NULL) {                     
        continue;
      }

        if (strcmp(args[0], "cd") == 0) {           //changes directory
          cd_cmd(args);

        }else if (strcmp(args[0], "clr") == 0) {    // clears screen
          clr_cmd();

        }else if (strcmp(args[0], "pause") == 0) {   // pauses operation until enter is pressed
          pause_cmd();

        }else if (strcmp(args[0], "quit") == 0) {    // quits shell
          break;

        }else {      // all other commands handled in child process
          run_cmd(args, background, inputf, outputf, append, shellpath);
        }

     }

return 0; //exits shell
}

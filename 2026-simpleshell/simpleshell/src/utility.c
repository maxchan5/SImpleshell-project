// utility.c contains the implementation of the built in commands used by simpleshell

#include "simpleshell.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;  // allows access to the environment variable list

void cd_cmd(char **args) {

  if (args[1] == NULL) {     // checks if no directory was provided
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {  //gets the current working and prints it
      printf("%s\n", cwd);
    }
  }else {
  // tries to change the directory
    if (chdir(args[1]) != 0) {
      perror("cd error");          // prints error if fails
    }else {

      char cwd[PATH_MAX];
      getcwd(cwd, sizeof(cwd));
      setenv("PWD", cwd, 1);       //if successful, PWD environment variable is updated
    }
  }
}

void clr_cmd() {         // clears terminal screen

  pid_t pid = fork();

  if (pid == 0) {                        // child process
    execlp("clear", "clear", NULL);      // execute clear command
    perror("clear failed");              // runs is execlp fails
    exit(1);
  }else {
    waitpid(pid, NULL, 0);                // wait for child to fiish
  }
}

void dir_cmd(char **args) {        // lists directory contents

  pid_t pid = fork();
      if (pid == 0) {
        if (args[1] == NULL) {
          execlp("ls", "ls", "-al", NULL);              // lists current directory
        }else {
          execlp("ls", "ls", "-al", args[1], NULL);     // lists specific directory
        }

        perror("dir failed");
        exit(1);
       }else {
         waitpid(pid, NULL, 0);
       }      
}

void environ_cmd() {     //lists all environment strings

  for (int i = 0; environ[i] != NULL; i++) {
    printf("%s\n", environ[i]);                //prints each string on a new line
  }
}


void echo_cmd(char **args) {    //prints text entered after "echo"

  for (int i = 1; args[i] != NULL; i++) {
    printf("%s ", args[i]);
  }
  printf("\n");                    //new line once printing is done
}

void help_cmd() {   //displays manual

  pid_t pid = fork();

  if (pid == 0) {
    execlp("more", "more", "../manual/readme.txt", NULL);
    perror("help failed");
    exit(1); 
  }else {
    waitpid(pid, NULL, 0);
  }
}

void pause_cmd() {  //pauses the operation until Enter is pressed

  printf("Press Enter to continue");
  while(getchar() != '\n');           //waits until Enter is pressed
}

void run_cmd(char **args, int background, char *inputf, char *outputf, int append, char *shellpath) {

  pid_t pid = fork();

  if (pid < 0) {             // if fork fails
    perror("fork failed");
    return;
  }

  if (pid == 0) {              //child process

    setenv("parent", shellpath, 1);                  //stores shell path in environment

    if (inputf != NULL) {                            // input redirection
      if (freopen(inputf, "r", stdin) == NULL) {
        perror("input redirection failed");
        exit(1);
      }
    }

    if (outputf != NULL) {                            // output redirection
    
      FILE *f;
      if (append) {
        f = freopen(outputf, "a", stdout);            //append
      }else {
        f = freopen(outputf, "w", stdout);            //overwrite
      }

      if (f == NULL) {
        perror("output redirection failed");
        exit(1);
      }
    }

    if (!strcmp(args[0], "dir")) {          // lists directory contents
            dir_cmd(args);
            exit(0);
          
    }else if (!strcmp(args[0], "environ")) { // list all the environment strings
            environ_cmd();
            exit(0);

    }else if (!strcmp(args[0], "echo")) {    // prints text entered after "echo"
            echo_cmd(args);
            exit(0);

    }else if (!strcmp(args[0], "help")) {    // displays manual
            help_cmd();
            exit(0);
    }

    execvp(args[0], args);     // execute external command
    

    perror("command execution failed");        //if execvp fails
    exit(1);
  }

  else {                                //parent process
    if (!background) {
      waitpid(pid, NULL, 0);
    }
  }
}


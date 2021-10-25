//Yasmine Watkins 
//CIS 3207 - 001
//Project 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"


int inBackground = FALSE; 
int redirectIn = FALSE;
int redirectOut = FALSE;
int infile, outfile;
int needspipe = FALSE;

//Takes the output out of the first command and uses as input for the second command
void execPiped(char **myargv, char **myargvpiped) { 
    int pipefd[2]; 
    pid_t p1, p2;
  
    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
    }
    p1 = fork();

    if (p1 < 0) {
        printf("\nCould not fork");
    }
  
    if (p1 == 0) {
        //Execution of child 1
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
  
        if (execvp(myargv[0], myargv) < 0) {
            printf("\nCould not execute first command.");
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();
  
        if (p2 < 0) {
            printf("\nCould not fork");
        }
  
        // Execution of child 2
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);

            
            if (execvp(myargvpiped[0], myargvpiped) < 0) {
                printf("\nCould not execute second command");
                exit(0);
            }
        } else {
            // closing both forks and parent executing, waiting for two children
            close(pipefd[0]);
            close(pipefd[1]);
            wait(NULL);
            wait(NULL);
        }
    }
}

void redirect(char **args){ 
    int i = 0;

    if(input(args) == 1) {
        // Builtins ran.
        needspipe = TRUE;
    } else {
        while (args[i] != NULL && i < MAX_ARGS){
        //Checking for background execution with "&" command
        if (strcmp(args[i], "&") == 0){
            args[i] = NULL;
            inBackground = TRUE;
            puts("BACKGROUND EXECUTION");
            break;
        } 
        //output redirection ">"
        else if (strcmp(args[i], ">") == FALSE) {
            redirectOut = TRUE;
            args[i] = NULL;
            outfile = open(args[i+1], O_WRONLY | O_TRUNC | O_CREAT | S_IRWXU);
        }
        //output append ">>"
        else if(strcmp(args[i], ">>") == FALSE) {
            redirectOut = TRUE; 
            args[i] = NULL;
            outfile = open(args[i+1], O_WRONLY | O_APPEND | O_CREAT | S_IRWXU);

        }
        // //input redirection "<"
        else if (strcmp(args[i], "<") == FALSE) {
            redirectIn = TRUE;
            args[i] = NULL;
            
            infile = open(args[i+1], O_RDONLY);
        }
        //This handles "|" for pipe 
        else if(strcmp(args[i], "|") == FALSE) {
            needspipe = TRUE;
            // Seperate args before and after "|"
            char *args1[MAX_ARGS]; 
            char *args2[MAX_ARGS]; 
            int j = 0; 

            while(j < i){ 
                args1[j] = args[j]; 
                j++; 
            }
            args1[j] = NULL;
            j = 0;
            i++;
            while(args[i] != NULL){ 
                args2[j] = args[i]; 
                j++;
                i++; 
            }
            args2[j] = NULL;
            execPiped(args1, args2);
        }
        
        i++;
    }
    }
    
}
//Executes system commands
void execArgs(char **myargv){ 
    int childPid; 
    childPid = fork();
    if (childPid < 0) {    
        printf("error \n");
    }
    else if (childPid == 0) {
        if(redirectIn == 1) {
            dup2(infile, STDIN_FILENO);
        }
        if(redirectOut == TRUE) {
            dup2(outfile, STDOUT_FILENO);
        }
        execvp(myargv[0], myargv); 
        printf("Error with command or executable\n");
    }  
    else {   
        if(inBackground == FALSE){ 
                waitpid(childPid, NULL, 0);
            }
    }
} 

int main(int argc, char const *argv[]) {
    //Implementing batch file
    FILE *input;
    int prompt = TRUE;

    if (argc == 1) {    // ./myshell
        input = stdin;
    } else if (argc == 2) {     // ./myshell <filename>
        input = fopen(argv[1], "r");
        if (input == NULL) {
            printf("Error\n");
            exit(1);
        }
        printf("file found\n");
        prompt = FALSE;
    }  else {   // ./myshell file file
        printf("Error\n");
        exit(1);
    }
    //Parsing to separate strings by spaces 
    while (1) {
        size_t index = 0,           
        n = 0;                 
        ssize_t nchr = 0;           
        char *line = NULL, *myargv[buffer] = {NULL};  
        
        if (prompt == TRUE) {
            fputs ("myShell >> ", stdout);
        }
        if ((nchr = getline (&line, &n, input)) == -1) {
            putchar ('\n');         
            break;
        }
        for (char *a = strtok (line, " \n"); a; a = strtok (NULL, " \n")) {
            myargv[index] = a;   
            //printf ("%s\n", myargv[index++]);
            if (index == buffer - 1)  
                break;
            index++;
        }
        redirect(myargv);
        if (needspipe == FALSE) {
            execArgs(myargv);
        } else {
            // Reset to false after pipe executes.
            needspipe = FALSE;
        }
        
        free(line); 
    } 
    return 0; 
}
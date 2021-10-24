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

#define TRUE 1
#define FALSE 0
#define buffer 256
#define MAX_ARGS 20

int inBackground = FALSE; 
int redirectIn = FALSE;
int redirectOut = FALSE;
int infile, outfile;
int needspipe = FALSE;

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
            printf("\nCould not execute command 1..");
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
                printf("\nCould not execute command 2..");
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
    while (args[i] != NULL && i < MAX_ARGS){
        //output redirection ">"
        if (strcmp(args[i], ">") == 0) {
            redirectOut = TRUE;
            args[i] = NULL;
            outfile = open(args[i+1], O_WRONLY | O_TRUNC | O_CREAT);
        }
        //output append ">>"
        else if(strcmp(args[i], ">>") == 0) {
            redirectOut = TRUE; 
            args[i] = NULL;
            outfile = open(args[i+1], O_WRONLY | O_APPEND | O_CREAT);

        }
        // //input redirection "<"
        else if (strcmp(args[i], "<") == 0) {
            redirectIn = TRUE;
            args[i] = NULL;
            
            infile = open(args[i+1], O_RDONLY);
        }
        //set | argument to null
        else if(strcmp(args[i], "|") == 0) {
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
            if(redirectOut == 1) {
                dup2(outfile, STDOUT_FILENO);
            }
            execvp(myargv[0], myargv); 
            printf("Error with command or executable\n");
            }
            else { 
            //if(inBackground == 0) {     
                waitpid(childPid, NULL, 0);
           // }
        }
}

int main(int argc, char const *argv[]) {
    while (1) {
        size_t index = 0,           
        n = 0;                 
        ssize_t nchr = 0;           
        char *line = NULL, *myargv[buffer] = {NULL};  

        fputs ("myShell >> ", stdout);
        if ((nchr = getline (&line, &n, stdin)) == -1) {
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
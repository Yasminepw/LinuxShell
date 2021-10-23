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


int r = TRUE; 
int inBg = 0; 
int redirectIn = 0;
int redirectOut = 0;
int infile, outfile;

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
         i++;
    }
    //next arg
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
            //if(inBg == 0) {     
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
        execArgs(myargv);
        free(line); 
    } 
    return 0; 
}
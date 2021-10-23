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

#define buffer 256
#define prompt "myShell >> "
#define newline " \n"
 
void execArgs(char **myargv) { 
    pid_t childPid; 
    childPid = fork();
        if (childPid < 0) {    
            printf("error \n");
        }
    
        else if (childPid == 0) {
            if (execvp(myargv[0], myargv) == -1) {
               printf("error\n");
            }

        } else {      
            waitpid(childPid, NULL, 0);
        }   
}
    
int main(int argc, char const *argv[]) {
   
    while (1) {
        size_t index = 0,           
        n = 0;                 
        ssize_t nchr = 0;           
        char *line = NULL, *myargv[buffer] = {NULL};  

        fputs (prompt, stdout);
        if ((nchr = getline (&line, &n, stdin)) == -1) {
            putchar ('\n');         
            break;
        }
        for (char *a = strtok (line, "\n"); a; a = strtok (NULL, "\n")) {
            myargv[index] = a;   
            //printf ("%s\n", myargv[index++]);
            if (index == buffer - 1)  
                break;
            index++;
        }
        execArgs(myargv);
        free(line); 
    } 
    return 0; 
}
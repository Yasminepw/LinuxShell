
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
 
    
int main(int argc, char const *argv[], char* execvp[]) {
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
        for (char *p = strtok (line, newline); p; p = strtok (NULL, newline)) {
            myargv[index] = p;   
            printf ("%s\n", myargv[index++]);
            if (index == buffer - 1)  
                break;
        }

        /* call to execvp, etc. here */

        free (line); 
    }

    return 0;
}

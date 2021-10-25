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

extern char **environ;

//clears the terminal
void clear(){
  printf("\033[H\033[J");
}

//change the current directory
void change_dir(char *newdir){
  if(chdir(newdir)){
    puts("Error: directory not found");
  }
}

void list_dir(char** args) {
    struct dirent *de;
    DIR *dr = opendir("."); 
    
    if (args[1] != NULL) {   
        dr = opendir(args[1]);
    }
    
    if(dr == NULL) {
        printf("Could not open current directory" );
    }

    while((de = readdir(dr)) != NULL) {
        printf("%s\n", de->d_name);
    }
    printf("\n"); 
    closedir(dr);
    
}

//list environment variable
void print_environ() {
    int i = 0;
    while(environ[i]) {
        printf("%s\n", environ[i++]);
    }
}

//returns the input as a string
void echo(char **args){
    args++;
  
     if (args!= NULL){
        printf("%s", *args);
        args++;

        while (*args != NULL){
            printf(" %s", *args);
            args++;
        }
    }
 
    puts("");
}

//prints help text 
void help() {
    FILE *fp;
    char c;
    //Opens and prints contents. 
    fp = fopen("README_DOC", "r");
    if(fp == NULL) {
        printf("Error opening file."); 
    }
    c = fgetc(fp);
    while (c != EOF) {
        printf ("%c", c);
        c = fgetc(fp);
    }
   printf("\n");
   fclose(fp);
}

//pauses the terminal until the enter key is pressed
void pause_command(){
    char *temp = getpass("Please press the enter key to continue.");
    free (temp);
}
//exit the program
void escape(){
    exit(0);
}
    
int input(char **args){

    //clear command
    if (!strcmp(args[0], "clr")) {
        clear();
        return TRUE;
    }
    //change directory
    else if(!strcmp(args[0], "cd")) {
        change_dir(args[1]);
        return TRUE;
    }
    //list command
    else if (!strcmp(args[0], "dir")) {
        list_dir(args);
        return TRUE; 
  }
    //environ
    else if (!strcmp(args[0], "environ")) {
        print_environ();
        return TRUE; 
    }
    //echo command
    else if (!strcmp(args[0], "echo")) {
        echo(args);
        return TRUE;
    }
    // //help
    else if (!strcmp(args[0], "help")) {
        help();
        return TRUE;
    }
    // //pause 
    else if (!strcmp(args[0], "pause")) {
        pause_command();
        return TRUE;
    }
    //exit command
    else if (!strcmp(args[0], "quit") ) {
        escape();
        return TRUE;
    }
    return 0;

}
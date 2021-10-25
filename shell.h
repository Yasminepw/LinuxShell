
#define TRUE 1
#define FALSE 0
#define buffer 256
#define MAX_ARGS 20

int input(char **args); 
void clear(); 
void change_dir(char *newdir);  
void list_dir(char** args); 
void print_environ(); 
void echo(char **args); 
void help(); 
void pause_command(); 
void escape(); 

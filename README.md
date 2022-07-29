# Linux Shell

My program is a simple command line shell that executes commands (either built in or through an enviroment path) 
read from the standard input or from a file. I had to change a lot of what I originally had in my psudocode because 
I found ways to impliment functions in an easier and more efficient way. I tested my program after implimentation of each new command 
and found it to be clearer by creating a separate file to place my builtins in.

For example the file test.txt is what was used as a batch file test.

## HELP MANUAL 
**************************************************************
LIST OF SUPPORTED BUILT IN COMMANDS
1. clr - clears the terminal
2. cd - Changes current directory 
3. dir - Lists the contents of the current directory 
4. environ - Prints out the enviroment variables 
5. echo - Prints args 
6. pause - Pauses the shell until user presses enter 
7. quit - Exits the terminal 
***************************************************************
FEATURES
1. Piping "|" - The standard output from one process becomes the standard input of the other process. 
   General syntax: cmd1 args | cmd2 args
   Example: ls -l | grep shell --> This will take the output of ls -l and use it as input for grep shell (whih finds all file names with "shell" and lists them) 
2. I/O Redirection - Changes the way from where commands read input to where commands sends output. 
    A. Syntax: command > file 
       Example: ls > test.txt - This will overwrite whatever is currently in test.txt with what is outputted from ls
    B. Syntax: command >> file
       Example: ls >> test.txt - This will take the output of ls and add it to what is in test.txt 
    C. Syntax: command < file - Takes a file and turns it into input redirection 
3. Background Execution - Running processes in the background by using "&" command 
   General syntax: cmd &
   Example: ls & - This will run the command ls in the background and user will press enter to continue

myShell: myshell.c builtins.c shell.h
		gcc -o myshell myshell.c builtins.c -Wall -Werror

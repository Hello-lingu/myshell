CC = gcc
CFALGS = -g -std=c2x

myshell: myshell.o builtin_command.o
	$(CC) $^ -o $@

myshell.o: myshell.c
	$(CC) $(CFALGS) $< -c $@

builtin_command.o: builtin_command.c
	$(CC) $(CFALGS) $< -c $@

CC = gcc
CFLAGS = -Wall -ansi -pedantic --std=gnu99

OBJS = colors.o myls.o mysh.o

myls: colors.o myls.o
	gcc -o myls colors.o myls.o

mysh: mysh.o
	gcc -o mysh mysh.o

colors.o: colors.c colors.h
	$(CC) $(CFLAGS) -c -o $@ $<

myls.o: myls.c myls.h
	$(CC) $(CFLAGS) -c -o $@ $<

mysh.o: mysh.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
.PHONY: clean

clean:
	@rm -f *.o
	@rm -f myls
	@rm -f mysh
	@echo "Clean done"
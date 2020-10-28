CC = gcc
CFLAGS = -Wall -ansi -pedantic --std=gnu99

OBJS = colors.o myls.o mysh.o

.PHONY: clean

myls: colors.o myls.o
	gcc -o myls colors.o myls.o

mysh: mysh.o
	gcc -o mysh mysh.o

myps: myps.o
	gcc -o myps myps.o

colors.o: colors.c colors.h
	$(CC) $(CFLAGS) -c -o $@ $<

myls.o: myls.c myls.h
	$(CC) $(CFLAGS) -c -o $@ $<

mysh.o: mysh.c mysh.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
myps.o: myps.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o
	@rm -f myls
	@rm -f mysh
	@rm -f myps
	@echo "Clean done"
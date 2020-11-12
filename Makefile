BINDIR = ./bin/
INCDIR = ./includes/
SRCDIR = ./src/
CC = gcc
CFLAGS = -Wall -ansi -pedantic --std=gnu99

.PHONY: all
all: myps myls mysh

utils.o: $(SRCDIR)utils.c $(INCDIR)utils.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

colors.o: $(SRCDIR)colors.c $(INCDIR)colors.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myls.o: $(SRCDIR)myls.c $(INCDIR)myls.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

mysh.o: $(SRCDIR)mysh.c $(INCDIR)mysh.h $(INCDIR)utils.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<
	
myps.o: $(SRCDIR)myps.c $(INCDIR)myps.h $(INCDIR)utils.h $(INCDIR)colors.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myls: colors.o  myls.o
	gcc -o $@ $(BINDIR)colors.o $(BINDIR)myls.o

mysh: utils.o mysh.o 
	gcc -o $@ $(BINDIR)utils.o $(BINDIR)mysh.o

myps: colors.o utils.o myps.o
	gcc -o $@ $(BINDIR)colors.o $(BINDIR)utils.o $(BINDIR)myps.o

.PHONY: clean
clean:
	@rm -f ./bin/*.o
	@rm -f myls
	@rm -f mysh
	@rm -f myps
	@echo "Clean done"
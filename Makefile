BINDIR = ./bin/
INCDIR = ./includes/
SRCDIR = ./src/
CC = gcc
CFLAGS = -Wall -ansi -pedantic --std=gnu99

TOOLS_H = $(INCDIR)utils.h $(INCDIR)colors.h
TOOLS_O = $(BINDIR)utils.o $(BINDIR)colors.o

.PHONY: all
all: myps myls mysh

utils.o: $(SRCDIR)utils.c $(INCDIR)utils.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

colors.o: $(SRCDIR)colors.c $(INCDIR)colors.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

redirections.o: $(SRCDIR)redirections.c $(INCDIR)redirections.h $(INCDIR)utils.h $(INCDIR)mysh.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myls.o: $(SRCDIR)myls.c $(INCDIR)myls.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

mysh.o: $(SRCDIR)mysh.c $(INCDIR)mysh.h $(TOOLS_H) $(INCDIR)redirections.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<
	
myps.o: $(SRCDIR)myps.c $(INCDIR)myps.h $(INCDIR)utils.h $(INCDIR)colors.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myls: colors.o  myls.o
	gcc -o $@ $(BINDIR)colors.o $(BINDIR)myls.o

mysh: colors.o utils.o redirections.o mysh.o 
	gcc -o $@ $(TOOLS_O) $(BINDIR)redirections.o $(BINDIR)mysh.o

myps: colors.o utils.o myps.o
	gcc -o $@ $(TOOLS_O) $(BINDIR)myps.o

.PHONY: clean
clean:
	@rm -f ./bin/*.o
	@rm -f myls
	@rm -f mysh
	@rm -f myps
	@echo "Clean done"
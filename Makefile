BINDIR = ./bin/
INCDIR = ./includes/
SRCDIR = ./src/
CC = gcc
CFLAGS = -Wall -ansi -pedantic --std=gnu99

TOOLS_H = $(INCDIR)utils.h $(INCDIR)colors.h
TOOLS_O = $(BINDIR)utils.o $(BINDIR)colors.o
TOOLS_R = utils.o colors.o

MYSH_H  = $(TOOLS_H) $(INCDIR)redirections.h $(INCDIR)myls.h $(INCDIR)myps.h $(INCDIR)builtin.h 
MYSH_O  = $(TOOLS_O) $(BINDIR)redirections.o $(BINDIR)myls_fct.o $(BINDIR)myps_fct.o $(BINDIR)builtin.o
MYSH_R  = $(TOOLS_R) redirections.o myls_fct.o myps_fct.o builtin.o

.PHONY: all
all: myps myls mysh

utils.o: $(SRCDIR)utils.c $(INCDIR)utils.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

colors.o: $(SRCDIR)colors.c $(INCDIR)colors.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

redirections.o: $(SRCDIR)redirections.c $(INCDIR)redirections.h $(INCDIR)utils.h $(INCDIR)mysh.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

builtin.o: $(SRCDIR)builtin.c $(INCDIR)utils.h $(INCDIR)myls.h $(INCDIR)myps.h $(INCDIR)builtin.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myls_fct.o: $(SRCDIR)myls_fct.c $(INCDIR)myls.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myls.o: $(SRCDIR)myls.c $(INCDIR)myls.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

mysh.o: $(SRCDIR)mysh.c $(INCDIR)mysh.h $(MYSH_H)
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<
	
myps_fct.o: $(SRCDIR)myps_fct.c $(INCDIR)myps.h $(TOOLS_H)
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myps.o: $(SRCDIR)myps.c $(INCDIR)myps.h $(TOOLS_H)
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<





myls: colors.o  myls.o myls_fct.o
	gcc -o $@ $(BINDIR)colors.o $(BINDIR)myls.o $(BINDIR)myls_fct.o

mysh: $(MYSH_R) mysh.o 
	gcc -o $@ $(MYSH_O) $(BINDIR)mysh.o

myps: colors.o utils.o myps.o myps_fct.o
	gcc -o $@ $(TOOLS_O) $(BINDIR)myps.o $(BINDIR)myps_fct.o

.PHONY: clean
clean:
	@rm -f ./bin/*.o
	@rm -f myls
	@rm -f mysh
	@rm -f myps
	@echo "Clean done"
BINDIR = ./bin/
INCDIR = ./includes/
SRCDIR = ./src/
CC = gcc
CFLAGS = -Wall -ansi -pedantic --std=c99

TOOLS_H = $(INCDIR)utils.h $(INCDIR)colors.h
TOOLS_O = $(BINDIR)utils.o $(BINDIR)colors.o
TOOLS_R = utils.o colors.o

MYSH_H  = $(TOOLS_H) $(INCDIR)redirections.h $(INCDIR)myls.h $(INCDIR)myps.h $(INCDIR)builtin.h 
MYSH_O  = $(TOOLS_O) $(BINDIR)redirections.o $(BINDIR)myls_fct.o $(BINDIR)myps_fct.o $(BINDIR)builtin.o $(BINDIR)mysh.o
MYSH_R  = $(TOOLS_R) redirections.o myls_fct.o myps_fct.o builtin.o mysh.o

.PHONY: all res
all: myps myls mysh
res: myssh myssh-server mysshd

utils.o: $(SRCDIR)utils.c $(INCDIR)utils.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

colors.o: $(SRCDIR)colors.c $(INCDIR)colors.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

redirections.o: $(SRCDIR)redirections.c $(INCDIR)redirections.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

builtin.o: $(SRCDIR)builtin.c $(INCDIR)builtin.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myls_fct.o: $(SRCDIR)myls_fct.c $(INCDIR)myls.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myls.o: $(SRCDIR)myls.c $(INCDIR)myls.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

mysh.o: $(SRCDIR)mysh.c $(INCDIR)mysh.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<
	
myps_fct.o: $(SRCDIR)myps_fct.c $(INCDIR)myps.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myps.o: $(SRCDIR)myps.c $(INCDIR)myps.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<



error.o: $(SRCDIR)error.c $(INCDIR)error.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myssh_main.o: $(SRCDIR)myssh_main.c $(INCDIR)myssh.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myssh.o: $(SRCDIR)myssh.c $(INCDIR)myssh.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

mysshd.o: $(SRCDIR)mysshd.c $(SRCDIR)myssh-server.c $(INCDIR)myssh-server.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myssh-server_main.o: $(SRCDIR)myssh-server_main.c $(INCDIR)myssh-server.h
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<

myssh-server.o: $(SRCDIR)myssh-server.c $(INCDIR)myssh-server.h 
	$(CC) $(CFLAGS) -c -o $(BINDIR)$@ $<



myssh: error.o myssh.o myssh_main.o 
	$(CC) $(CFLAGS) -o $@ $(BINDIR)myssh.o $(BINDIR)myssh_main.o $(BINDIR)error.o

mysshd: error.o myssh-server.o mysshd.o
	$(CC) $(CFLAGS) -o $@ $(BINDIR)error.o $(BINDIR)myssh-server.o $(BINDIR)mysshd.o

myssh-server: error.o myssh-server.o myssh-server_main.o
	$(CC) $(CFLAGS) -o $@ $(BINDIR)error.o $(BINDIR)myssh-server.o $(BINDIR)myssh-server_main.o -lcrypt
	sudo chown root $@
	sudo chmod a+s $@


myls: colors.o myls.o myls_fct.o
	$(CC) $(CFLAGS) -o $@ $(BINDIR)colors.o $(BINDIR)myls.o $(BINDIR)myls_fct.o

mysh: $(MYSH_R)
	$(CC) $(CFLAGS) -o $@ $(MYSH_O)

myps: colors.o utils.o myps.o myps_fct.o
	$(CC) $(CFLAGS) -o $@ $(TOOLS_O) $(BINDIR)myps.o $(BINDIR)myps_fct.o



.PHONY: clean
clean:
	@rm -f ./bin/*.o
	@rm -f myls mysh myps myssh mysshd myssh-server
	@echo "Clean done"
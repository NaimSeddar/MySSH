#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/dir.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <string.h>
#include <time.h>

#define ERR -1

typedef struct procInfo
{
    char *user;
    int pid;
    float cpu;
    float mem;
    int vsz;
    int rss;
    char *tty;
    char *stat;
    char *start;
    char *time;
    char *command;
} proc;

int getmemtotal();
void getstatus(char *pid);
void getcmd(char *pid, proc *p);
void getstart(struct stat s);
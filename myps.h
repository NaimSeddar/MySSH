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

typedef struct _proc
{
    char *user;
    char *pid;
    float cpu;
    float mem;
    int vsz;
    int rss;
    char *tty;
    char *stat;
    char *start;
    char *time;
    char *command;
    int threads;
    int lock;
    int time_cumul;
} proc;

char **str_split(char *str, const char delimiter);
int getmemtotal();
void getstatus(char *pid);
void getcmd(char *pid, proc *p);
void getstart(struct stat s, proc *p);
void print_proc(proc *p);
/**
 * Auteur:                Seddar Naïm
 * Création:              08/11/2020 14:10:29
 * Dernière modification: 30/12/2020 15:23:30
 * Master 1 Informatique
 */

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

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

/**
 * Get the uptime value in /proc/uptime.
 * @return  The uptime.
 */
float getuptime();

/**
 * Get the value from field 'MemTotal' in /proc/meminfo.
 * @return  The MemTotal field. 
 */
int getmemtotal();

/**
 * Store the memory use.
 * @param  p  proc struct var where we store wanted values.
 */
void getmem(proc *p);

/**
 * Get the command line from /proc/[pid]/cmdline.
 * @param  pid  Process ID.
 * @param  p    proc struct var where we store wanted values.
 */
void getcmd(char *pid, proc *p);

/**
 * Get the start value.
 * @param  s  stat struct with states from proc directory.
 * @param  p  proc struct var where we store wanted values.
 */
void getstart(struct stat s, proc *p);

/**
 * Parse the /proc/[pid]/status file.
 * @param  p  proc struct var where we store wanted values.
 */
void parse_status(proc *p);

/**
 * Parse the /proc/[pid]/stat file.
 * @param  p  proc struct var where we store wanted values.
 */
void parse_stat(proc *p);

/**
 * Apply the color associated with the process state.
 * @param  p  proc struct var where we store wanted values.
 */
void getcolor(proc *p);

/**
 * Print every fields.
 * @param  p  proc struct var where we store wanted values.
 */
void print_proc(proc *p);

/**
 * Read the /proc/ directory.
 * @return  0 on success, -1 if there's an error.
 */
int myps();
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pwd.h>
#include "colors.h"

#define ERR -1
#define FAILEDEXEC 127
#define MAX_DIR 256

extern char *perms[];

const char *userperms(mode_t m);

const char *grpperms(mode_t m);

const char *otherperms(mode_t m);

char dirType(mode_t m);

void print_dirHeader(char *dir);

void print_perms(struct stat s);

void print_date(struct stat s);

void print_user(struct stat s);

void print_line(struct stat s, char *name);

void myls(char *path, int a, int r);
/**
 * Auteur:                Seddar Naïm
 * Création:              27/10/2020 16:30:43
 * Dernière modification: 20/12/2020 12:22:42
 * Master 1 Informatique
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
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

void ls(char *path, int a, int r);

int myls(char **paths);
/**
 * Auteur:                Seddar Naïm
 * Création:              27/10/2020 16:30:43
 * Dernière modification: 30/12/2020 15:07:27
 * Master 1 Informatique
 */

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
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

/* Array with every permissions */
extern char *perms[];

/**
 * Get the user permissions.
 * @param  m  Permission in bits.
 * @return    A string corresponding to the permissions.
 */
const char *userperms(mode_t m);

/**
 * Get the group permissions.
 * @param  m  Permission in bits.
 * @return    A string corresponding to the permissions.
 */
const char *grpperms(mode_t m);

/**
 * Get the other permissions.
 * @param  m  Permission in bits.
 * @return    A string corresponding to the permissions.
 */
const char *otherperms(mode_t m);

/**
 * Get file/dir type.
 * @param  m  Type in bits.
 * @return    A string corresponding to the permissions.
 */
char dirType(mode_t m);

/**
 * Print the directory header.
 * @param  dir  The directory.
 */
void print_dirHeader(char *dir);

/**
 * Print the file permissions.
 * @param  s  stat struct containing every permissions.
 */
void print_perms(struct stat s);

/**
 * Print the file date.
 * @param  s  stat struct containing the date.
 */
void print_date(struct stat s);

/**
 * Print the file user owner.
 * @param  s  stat struct containing the owner.
 */
void print_user(struct stat s);

/**
 * Print the file group owner.
 * @param  s  stat struct containing the owner.
 */
void print_group(struct stat s);

/**
 * Print the file permissions, date, user and group.
 * @param  s     stat struct containing every data.
 * @param  name  File name.
 */
void print_line(struct stat s, char *name);

/**
 * Do an ls -l on the given path.
 * @param  a  if a > 0, shows hidden files.
 * @param  r  if r > 0, do an ls on subdirectories.
 */
void ls(char *path, int a, int r);

/**
 * Count and launch an ls with the proper parameters.
 * @param  paths  The different paths and args in the command line.
 * @return        Always 0.
 */
int myls(char **paths);
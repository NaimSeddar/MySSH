/**
 * Auteur:                Seddar Naïm
 * Création:              21/11/2020 13:23:15
 * Dernière modification: 20/12/2020 12:22:23
 * Master 1 Informatique
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int cd(char *path);

int builtin_parser(char **command_line);
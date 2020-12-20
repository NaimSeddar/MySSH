/**
 * Auteur:                Seddar Naïm
 * Création:              16/11/2020 12:04:23
 * Dernière modification: 20/12/2020 12:23:41
 * Master 1 Informatique
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int stdout_to_fic(char *command, int flags);

int stderr_to_fic(char *command, int flags);

int stderr_and_stdout(char *command, int flags);

int fic_to_stdin(char *command);
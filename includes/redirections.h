/**
 * Auteur:                Seddar Naïm
 * Création:              16/11/2020 12:04:23
 * Dernière modification: 30/12/2020 13:02:30
 * Master 1 Informatique
 */

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * Redirect the standard output into a specified file.
 * @param  command  The command line.
 * @param  flags    Flags used to open the file (Append or truncate).
 * @return          0 on success, else 1.
 */
int stdout_to_fic(char *command, int flags);

/**
 * Redirect the standard error into a specified file.
 * @param  command  The command line.
 * @param  flags    Flags used to open the file (Append or truncate).
 * @return          0 on success, else 1.
 */
int stderr_to_fic(char *command, int flags);

/**
 * Redirect both, standard error and output into a specified file.
 * @param  command  The command line.
 * @param  flags    Flags used to open the file (Append or truncate).
 * @return          0 on success, else 1.
 */
int stderr_and_stdout(char *command, int flags);

/**
 * Redirect standard input into a specified command.
 * @param  command  The command line.
 * @return          0 on success, else 1.
 */
int fic_to_stdin(char *command);
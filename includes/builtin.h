/**
 * Auteur:                Seddar Naïm
 * Création:              21/11/2020 13:23:15
 * Dernière modification: 30/12/2020 11:55:11
 * Master 1 Informatique
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>

/**
 * Make a foreground or a stopped job run in background.
 * @param  var  The job id as a string (can be null). 
 * @return      2 if something went wrong, else 0.
 */
int mybg(char *var);

/**
 * Make a background or a stopped job run in foreground.
 * @param  var  The job id as a string (can be null).
 * @return      2 if something went wrong, else 0.
 */
int myfg(char *var);

/**
 * Give the previous foreground job status.
 * @param  var  The job id as a string (can be null).
 * @return      0
 */
int status();

/**
 * Print the list of all the current jobs.
 * @return 0
 */
int myjobs();

/**
 * Clear the terminal.
 * @return 0
 */
int clear_prompt();

/**
 * Change the current directory.
 * @param  path  The path that we want to take. If null, goes to the home directory.
 * @return       If something went wrong, it returns 2, else 0. 
 */
int cd(char *path);

/**
 * Set a variable.
 * @param  var  Variable assignement.
 * @return      Returns 0 if everything's fine, else 2.
 */
int set_var(char *var);

/**
 * Unset a variable.
 * @param  var  Variable's name.
 * @return      Returns 0 if everything's fine, else 2.
 */
int unset_var(char *var);

/**
 * Parses and launch the proper function according to the command line
 * @param  command_line  A command line
 * @return               One of the exit code from built in function (0 or 2), else -1
 */
int builtin_parser(char **command_line);
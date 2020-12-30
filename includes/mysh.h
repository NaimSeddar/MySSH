/**
 * Auteur:                Seddar Naïm
 * Création:              28/10/2020 10:19:34
 * Dernière modification: 30/12/2020 14:32:45
 * Master 1 Informatique
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <glob.h>
#include <pwd.h>

#include "../includes/utils.h"
#include "../includes/colors.h"
#include "../includes/redirections.h"
#include "../includes/builtin.h"

#define BUFFER_SIZE 1024
#define ERR -1
#define FAILED_EXEC 127

struct myjob
{
    int show;
    int job_id;
    int pid;
    char command[256];
    const char *etat;
};

/* Pid from the current command */
extern pid_t cmd_pid;

/* Status of the last command */
extern int pcode;

/* Array of jobs */
extern struct myjob jobs[256];

/* Number of jobs */
extern int nb_jobs;

/* Previous command */
extern char prev_cmd[4096];

/* Pid from the previous foreground command */
extern pid_t prev_fg_proc;

/* Previous foreground command status */
extern int prev_pcode;

/**
 * SIGINT handler.
 * @param  sig  The signal.
 */
void ctrlc(int sig);

/**
 *  SIGTSTP handler.
 * @param  sig  The signal.
 */
void ctrlz(int sig);

/**
 * Check the status of every jobs.
 */
void check_jobs();

/**
 * Check if there's an '&' at the last field.
 * @param  fields  Every fields of the command line.
 * @return         1 if there's an '&' at the end, otherwise, 0.
 */
int run_it_in_bg(char **fields);

/**
 * A better and lighter version of system() base on execvp, saw in class.
 * @param  command  The raw command line.
 * @return          The status of the executed command, -1 if an error occured.
 **/
int systemV2(char *command);

/**
 * Parse the command line and launch the proper function.
 * @param  command  The raw command line.
 * @return          The status of the executed function, -1 if an error occured.
 */
int parser(char *command);

/**
 * Do the && operation in the given command.
 * @param  command  Raw command line.
 * @return          0 on success, or the first failure status. -1 if an error occure.
 */
int and_op(char *command);

/**
 * Do the || operation in the given command.
 * @param  command  Raw command line.
 * @return          0 on success, or the last failure status. -1 if an error occure.
 */
int or_op(char *command);

/**
 * Apply the pipes in the given command.
 * @param  command  Raw command line.
 * @return          0 on success, -1 on failure.
 */
int pipeline(char *command);

/**
 * Search if in the given command line, there's some variables. If there is, it will replace them.
 * @param  commands  The command line as an string array.
 */
void search_replace_var(char **commands);

/**
 * Print a prompt for the user. ([username]:[path]$ )
 */
void printprompt();

/**
 * An infinite loop where the user can execute commands until he wants to quit.
 */
void mysh();

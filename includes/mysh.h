/**
 * Auteur:                Seddar Naïm
 * Création:              28/10/2020 10:19:34
 * Dernière modification: 29/12/2020 10:59:34
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
    int job_id;
    int pid;
    char command[256];
    char etat[32];
};

typedef struct myjob *Myjobs;

extern pid_t cmd_pid;
extern int pcode;
extern Myjobs jobs[256];
extern int nb_jobs;

/**
 * A better system(), saw in class.
 * A lighter version base on execvp. 
 * @param command
 * */
int systemV2(char *command);

int parser(char *command);

int and_op(char *command);

int or_op(char *command);

int pipeline(char *command);

void search_replace_var(char **commands);

void printprompt(int pcode);

void mysh();

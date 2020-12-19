#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <glob.h>

#include "../includes/utils.h"
#include "../includes/colors.h"
#include "../includes/redirections.h"
#include "../includes/builtin.h"

#define BUFFER_SIZE 1024
#define ERR -1
#define FAILED_EXEC 127

extern pid_t cmd_pid;

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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "../includes/utils.h"
#include "../includes/colors.h"
#include "../includes/redirections.h"

#define BUFFER_SIZE 1024
#define ERR -1
#define FAILED_EXEC 127

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

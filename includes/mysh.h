#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define ERR -1
#define FAILED_EXEC 127

/**
 * A better system(), saw in class.
 * A lighter version base on execvp. 
 * @param command
 * */
int systemV2(char **command);

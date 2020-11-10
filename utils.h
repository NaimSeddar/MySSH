#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE (1024 * sizeof(char))

/**
 * Découpe un string par un délimiteur donné, et retourne un tableau de string. 
 */
char **str_split(char *str, const char delimiter);

/*
 * File into string
 */

char *file_to_string(char *filename);
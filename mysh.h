#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define ERR -1
#define FAILED_EXEC 127

char **cmd_split(char *str, const char delimiter);

/**
 * Réécriture de la fonction system() vu en cours.
 * Version moins lourde, basé sur la fonction execl. 
 * @param command
 * */
int systemV2(char **command);

/**
 * Fonction vidant le buffer
 * @param buffer
 * */
void clearBuffer(char *buffer);
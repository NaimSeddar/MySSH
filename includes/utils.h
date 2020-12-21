/**
 * Auteur:                Seddar Naïm
 * Création:              09/11/2020 13:35:46
 * Dernière modification: 21/12/2020 12:55:28
 * Master 1 Informatique
 */

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE_MALLOC (1024 * sizeof(char))

/**
 * Découpe un string par un délimiteur donné, et retourne un tableau de string. 
 */
char **str_split(char *str, const char delimiter);

int occu(char *str, const char *needle);

char **str_splitv2(char *str, const char *delimiter);

void remove_whitespaces(char *str);

/*
 * File into string
 */
char *file_to_string(char *filename);
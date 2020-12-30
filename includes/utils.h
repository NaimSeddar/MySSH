/**
 * Auteur:                Seddar Naïm
 * Création:              09/11/2020 13:35:46
 * Dernière modification: 30/12/2020 12:47:27
 * Master 1 Informatique
 */

#ifndef UTILS
#define UTILS

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
 * Split a string by a delimiter (char).
 * @param  str        String to split.
 * @param  delimiter  The delimiter. 
 * @return            Returns the splitted string as an array of string.
 */
char **str_split(char *str, const char delimiter);

/**
 * Count how many times a substring appears in a string.
 * @param  str     The haystack.
 * @param  needle  The substring.
 * @return         Number of needle in str.
 */
int occu(char *str, const char *needle);

/**
 * Another version of str_split, but this time, we take a string as delimiter.
 * @param  str        String to split.
 * @param  delimiter  The delimiter.
 */
char **str_splitv2(char *str, const char *delimiter);

/**
 * Trim the string
 * @param  str  String to trim.
 */
void remove_whitespaces(char *str);

/**
 * Store the content of a file in a string buffer.
 * @param  filename  The file's name
 * @return           The file's content
 */
char *file_to_string(char *filename);

#endif
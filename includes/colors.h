/**
 * Auteur:                Seddar Naïm
 * Création:              25/10/2020 12:27:36
 * Dernière modification: 30/12/2020 12:01:55
 * Master 1 Informatique
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// [COLOR]_C
#define BLACK_C "\033[1;30m"
#define RED_C "\033[1;31m"
#define GREEN_C "\033[1;32m"
#define YELLOW_C "\033[1;33m"
#define BLUE_C "\033[1;34m"
#define MAGENTA_C "\033[1;35m"
#define CYAN_C "\033[1;36m"
#define WHITE_C "\033[1;37m"
#define RESET_C "\033[0m"

// Underline
#define U_RED_C "\033[4;31m"
#define U_WHITE_C "\033[4;37m"

/**
 * Put a gray background in the terminal.
 */
void grayBG();

/**
 * Put a white background in the terminal.
 */
void whiteBG();

/**
 * Write in the given color
 * @param  color  The color we want to use.
 */
void writein(const char *color);
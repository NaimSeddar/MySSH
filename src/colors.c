/**
 * Auteur:                Seddar Naïm
 * Création:              25/10/2020 13:07:23
 * Dernière modification: 20/12/2020 12:24:54
 * Master 1 Informatique
 */

#include "../includes/colors.h"

void grayBG()
{
    printf("\033[40m");
}

void whiteBG()
{
    printf("\033[47m");
}

void writein(const char *color)
{
    if (write(STDOUT_FILENO, color, sizeof(color)) == -1)
        perror("Write"), exit(1);
}
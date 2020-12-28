/**
 * Auteur:                Seddar Naïm
 * Création:              24/12/2020 13:10:55
 * Dernière modification: 28/12/2020 21:57:15
 * Master 1 Informatique
 */

#include "../includes/mysh.h"

int main(int argv, char *argc[])
{
    if (argv > 1)
    {
        printf("Aucun argument n'est requis pour '%s'\n", argc[1]);
        exit(1);
    }

    mysh();

    return 0;
}
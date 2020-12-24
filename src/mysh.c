/**
 * Auteur:                Seddar Naïm
 * Création:              24/12/2020 13:10:55
 * Dernière modification: 24/12/2020 14:26:43
 * Master 1 Informatique
 */

#include "../includes/mysh.h"

/* TODO
 *  Demander confirmation, et tuer tout les enfants en fond.
 */
void ctrlc(int sig)
{
    printf("pid to kill : %d (from %d) %d\n", getpid(), getppid(), cmd_pid);
    if (cmd_pid == -1)
    {
        printf("kill main\n");
        kill(getpid(), SIGKILL);
    }
    else
    {
        kill(cmd_pid, SIGKILL);
    }
    cmd_pid = -1;
}

int main(int argv, char *argc[])
{
    if (argv > 1)
    {
        printf("Aucun argument n'est requis pour '%s'\n", argc[1]);
        exit(1);
    }

    signal(SIGINT, ctrlc);

    mysh();

    return 0;
}
#include "../includes/redirections.h"
#include "../includes/mysh.h"

/*
 * commande >> fichier
 */
int stdout_to_endoffic(char *command)
{
    if (occu(command, ">>") > 1)
        return 1;

    char **elt = str_splitv2(command, ">>");
    int out, save_out, res;

    remove_whitespaces(elt[1]);

    if ((out = open(elt[1], O_CREAT | O_APPEND | O_WRONLY)) < 0)
    {
        perror("open (>>)");
        return 1;
    }

    save_out = dup(fileno(stdout));

    if (dup2(out, fileno(stdout)) < 0)
    {
        perror("dup2 (>>)");
        return 1;
    }

    res = parser(elt[0]);

    fflush(stdout);
    close(out);

    dup2(save_out, fileno(stdout));

    close(save_out);

    return res;
}
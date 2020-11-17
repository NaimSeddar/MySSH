#include "../includes/redirections.h"
#include "../includes/mysh.h"

/*
 *  TODO:
 *      2>      idem que > pour la sortie standard des erreurs
 *      2>>     idem que >> pour la sortie standard des erreurs
 *      >&      redirige la stdin et la stderr de la commande vers le fichier. O_TRUNC 
 *      >>&     redirige la stdin et la stderr de la commande vers le fichier. O_APPEND 
 *      <       redirige l"entrée standard de la commande vers stdin 
 */

/*
 * commande >> fichier
 * flags:
 *  >>  O_CREAT | O_WRONLY | O_APPEND
 *  >   O_CREAT | O_WRONLY | O_TRUNC
 */
int stdout_to_fic(char *command, int flags)
{
    /*
     * Flags: 
     *      577  if O_TRUNC 
     *      1089 if O_APPEND 
     */
    if (occu(command, ">>") > 1)
        return 1;

    char **elt = str_splitv2(command, ">>");
    int out, save_out, res;

    remove_whitespaces(elt[1]);
    /* - rw- r-- r-- */
    if ((out = open(elt[1], flags, 0644)) < 0)
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

int stderr_to_fic(char *command, int flags)
{
    /*
     * Flags: 
     *      577  if O_TRUNC 
     *      1089 if O_APPEND 
     */

    const char *type = (flags == 577 ? "2>" : "2>>");
    if (occu(command, type) > 1)
        return 1;

    char **elt = str_splitv2(command, type);
    int err, save_err, res;

    remove_whitespaces(elt[1]);
    /* - rw- r-- r-- */
    if ((err = open(elt[1], flags, 0644)) < 0)
    {
        perror("open (stderr_to_fic)");
        return 1;
    }

    save_err = dup(fileno(stderr));

    if (dup2(err, fileno(stderr)) < 0)
    {
        perror("dup2 (stderr_to_fic)");
        return 1;
    }

    res = parser(elt[0]);

    fflush(stderr);
    close(err);

    dup2(save_err, fileno(stderr));

    close(save_err);

    return res;
}
/**
 * Auteur:                Seddar Naïm
 * Création:              16/11/2020 12:04:13
 * Dernière modification: 30/12/2020 12:54:18
 * Master 1 Informatique
 */

#include "../includes/redirections.h"
#include "../includes/mysh.h"

/*
 * commande >> fichier
 * flags:
 *  >>  O_CREAT | O_WRONLY | O_APPEND (1089)
 *  >   O_CREAT | O_WRONLY | O_TRUNC  (577)
 */

int stdout_to_fic(char *command, int flags)
{
    const char *type = (flags == 577 ? ">" : ">>");

    if (occu(command, type) > 1)
        return 1;

    char **elt = str_splitv2(command, type);
    int out, save_out, res;

    remove_whitespaces(elt[1]);
    /* - rw- r-- r-- */
    if ((out = open(elt[1], flags, 0644)) < 0)
    {
        perror("open (stdout to fic)");
        return 1;
    }

    save_out = dup(fileno(stdout));

    if (dup2(out, fileno(stdout)) < 0)
    {
        perror("dup2 (stdout to fic)");
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

int stderr_and_stdout(char *command, int flags)
{
    const char *type = (flags == 577 ? ">&" : ">>&");
    if (occu(command, type) > 1)
        return 1;

    char **elt = str_splitv2(command, type);
    int err, out, save_err, save_out, res;

    remove_whitespaces(elt[1]);
    /* - rw- r-- r-- */
    /* The out always append so there's both, stderr and stdout printed in the file */
    if ((err = open(elt[1], flags, 0644)) < 0 || (out = open(elt[1], O_WRONLY | O_APPEND, 0644)) < 0)
    {
        perror("open (stderr_and_stdout)");
        return 1;
    }

    save_err = dup(fileno(stderr));
    save_out = dup(fileno(stdout));

    if (dup2(err, fileno(stderr)) < 0 || dup2(out, fileno(stdout)) < 0)
    {
        perror("dup2 (stderr_and_stdout)");
        return 1;
    }

    res = parser(elt[0]);

    fflush(stdout);
    close(out);

    fflush(stderr);
    close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

    return res;
}

int fic_to_stdin(char *command)
{
    if (occu(command, "<") > 1)
        return 1;

    char **elt = str_splitv2(command, "<");
    int in, save_in, res;

    remove_whitespaces(elt[1]);

    if ((in = open(elt[1], O_CREAT | O_RDONLY)) < 0)
    {
        perror("open (<)");
        return 1;
    }

    save_in = dup(fileno(stdin));

    if (dup2(in, fileno(stdin)) < 0)
    {
        perror("dup2 (<)");
        return 1;
    }

    res = systemV2(elt[0]);

    fflush(stdin);

    close(in);

    dup2(save_in, fileno(stdin));
    close(save_in);

    return res;
}
/**
 * Auteur:                Seddar Naïm
 * Création:              21/11/2020 12:13:21
 * Dernière modification: 29/12/2020 13:27:39
 * Master 1 Informatique
 */

#include "../includes/builtin.h"
#include "../includes/utils.h"
#include "../includes/myls.h"
#include "../includes/myps.h"
#include "../includes/mysh.h"

int status()
{
    if (prev_fg_proc == -1)
    {
        printf("Aucune commande n'a encore été exécuté en premier plan\n");
        return 0;
    }
    else
    {
        printf("%d terminé ", prev_fg_proc);
    }

    if (prev_pcode != -1)
    {
        printf("avec comme code de retour %d\n", prev_pcode);
    }
    else
    {
        printf("anormalement\n");
    }

    return 0;
}

int myjobs()
{
    if (nb_jobs == 0)
        return 0;
    for (int i = 0; i < nb_jobs; i++)
    {
        if (jobs[i].show)
            printf("[%d] %d %s %s\n", jobs[i].job_id, jobs[i].pid, jobs[i].etat, jobs[i].command);
    }
    return 0;
}

int clear_prompt()
{
    printf("\33[H\33[J");
    return 0;
}

int cd(char *path)
{
    if (path == NULL)
    {
        path = getenv("HOME");
    }

    if (chdir(path) == ERR)
    {
        perror("cd");
    }

    return 0;
}

int set_var(char *var)
{
    if (var != NULL)
        return putenv(var);
    else
        return 1;
}

int unset_var(char *var)
{
    if (var != NULL)
        return unsetenv(var);
    else
        return 1;
}

int builtin_parser(char **command_line)
{
    remove_whitespaces(command_line[0]);
    if (strncmp(command_line[0], "cd", 2) == 0)
    {
        // remove_whitespaces(arg);
        return cd(command_line[1]);
    }
    else if (strncmp(command_line[0], "exit", 4) == 0)
    {
        exit(0);
    }
    else if (strncmp(command_line[0], "myls", 4) == 0)
    {
        // printf("Several path ! (%s)\n", arg);
        myls(++command_line);
        return 0;
    }
    else if (strncmp(command_line[0], "myps", 4) == 0)
    {
        return myps();
    }
    else if (strncmp(command_line[0], "set", 3) == 0)
    {
        return set_var(command_line[1]);
    }
    else if (strncmp(command_line[0], "unset", 5) == 0)
    {
        return unset_var(command_line[1]);
    }
    else if (strncmp(command_line[0], "clear", 5) == 0)
    {
        return clear_prompt();
    }
    else if (strncmp(command_line[0], "myjobs", 6) == 0)
    {
        return myjobs();
    }
    else if (strncmp(command_line[0], "status", 6) == 0)
    {
        return status();
    }

    return 1;
}

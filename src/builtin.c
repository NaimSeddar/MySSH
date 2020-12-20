/**
 * Auteur:                Seddar Naïm
 * Création:              21/11/2020 12:13:21
 * Dernière modification: 20/12/2020 12:24:45
 * Master 1 Informatique
 */

#include "../includes/builtin.h"
#include "../includes/utils.h"
#include "../includes/myls.h"
#include "../includes/myps.h"

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
    // free(path);
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

    return 1;
}

#include "../includes/builtin.h"
#include "../includes/utils.h"

int cd(char *path)
{
    if (path == NULL)
    {
        path = getenv("HOME");
    }
    return chdir(path);
}

int builtin_parser(char *command, char *arg)
{
    /*if (arg == NULL)
    {
        printf("NULL !\n");
        // exit(1);
    }*/
    remove_whitespaces(command);
    if (strncmp(command, "cd", 2) == 0)
    {
        // remove_whitespaces(arg);
        return cd(arg);
    }
    else if (strncmp(command, "exit", 4) == 0)
    {
        exit(0);
    }
    return 1;
}

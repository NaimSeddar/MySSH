#include "../includes/builtin.h"
#include "../includes/utils.h"
#include "../includes/myls.h"
#include "../includes/myps.h"

int cd(char *path)
{
    int res;
    if (path == NULL)
    {
        path = getenv("HOME");
    }
    res = chdir(path);
    // free(path);
    return res;
}

int builtin_parser(char **command_line)
{
    /*if (arg == NULL)
    {
        printf("NULL !\n");
        // exit(1);
    }*/
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
    return 1;
}

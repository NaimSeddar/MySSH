#include "../includes/redirections.h"

int stdout_to_endoffic(char *command)
{

    char **elt = str_splitv2(command, ">>");

    printf("v\n");
    while (*elt)
    {
        remove_whitespaces(*elt);
        printf("(%s)\n", *(elt++));
    }
    printf("^\n");

    // printf(">> avec (%s)\n", command);

    return 0;
}
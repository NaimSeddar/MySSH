#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include "colors.h"

int main(int argv, char *argc[])
{
    // char* getcwd(char *buf, size_t size)
    // char* getcwd(char *buf)

    char buffer[1024];
    char *m = getcwd(buffer, sizeof(buffer));
    red(m);

    return 0;
}
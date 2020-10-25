#include "colors.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argv, char *argc[])
{
    // char* getcwd(char *buf, size_t size)
    // char* getcwd(char *buf)

    char buffer[1024];
    char *path = getcwd(buffer, sizeof(buffer));
    printf(GREEN_C "%s\n" RESET_C, path);

    return 0;
}
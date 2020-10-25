#include "colors.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>

#define ERR -1

int main(int argv, char *argc[])
{
    // char* getcwd(char *buf, size_t size)
    // char* getcwd(char *buf)

    // char buffer[1024];
    // char *path = getcwd(buffer, sizeof(buffer));

    DIR *d = opendir("/proc");
    struct dirent *file;

    if (d == 0)
    {
        perror("Path");
        exit(1);
    }

    while ((file = readdir(d)) != 0)
    {
        printf("%s | %d\n", file->d_name, file->d_type);
    }

    if (closedir(d) == ERR)
        exit(ERR);

    return 0;
}
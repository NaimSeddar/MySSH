#include "colors.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define ERR -1

int main(int argv, char *argc[])
{
    // char* getcwd(char *buf, size_t size)
    // char* getcwd(char *buf)

    char buffer[1024];
    char *path = getcwd(buffer, sizeof(buffer));

    DIR *d = opendir(path);
    struct dirent *file;
    struct stat fileInfo;

    if (d == 0)
    {
        perror("Path");
        exit(1);
    }

    while ((file = readdir(d)) != 0)
    {
        strcat(buffer, file->d_name);
        stat(buffer, &fileInfo);
        printf("%d . %s / %o\n", file->d_type, file->d_name, fileInfo.st_mode);
    }

    if (closedir(d) == ERR)
        exit(ERR);

    return 0;
}
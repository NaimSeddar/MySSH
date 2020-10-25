#include "colors.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <errno.h>
#include <string.h>

#define ERR -1

#define str(x) #x

char dirType(int type)
{
    char res = ' ';
    switch (type)
    {
    case 4:
        res = '-';
        break;
    case 8:
        res = 'd';
        break;
    case 10:
        res = 'l';
        break;
    default:
        res = type + '0';
        break;
    }

    return res;
}

int main(int argv, char *argc[])
{
    // char* getcwd(char *buf, size_t size)
    // char* getcwd(char *buf)

    char buffer[1024] = "/proc/";
    // char *path = getcwd(buffer, sizeof(buffer));

    DIR *d = opendir(buffer);
    struct dirent *file;
    struct stat fileInfo;

    if (d == 0)
    {
        perror("Path");
        exit(1);
    }

    while ((file = readdir(d)) != 0)
    {
        strcpy(buffer, "/proc/");
        strcat(buffer, file->d_name);
        stat(buffer, &fileInfo);
        printf("%c %o  %s\n", dirType(file->d_type), fileInfo.st_mode, buffer);
    }

    if (closedir(d) == ERR)
        exit(ERR);

    return 0;
}
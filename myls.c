#include "colors.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define ERR -1

static const char *rwx[] = {
    "---", "--x", "-w-", "-wx",
    "r--", "r-x", "rw-", "rwx"};

char dirType(mode_t mode)
{
    char res = ' ';

    if (S_ISDIR(mode))
    {
        res = 'd';
    }
    else if (S_ISLNK(mode))
    {
        res = 'l';
    }
    else
    {
        res = '-';
    }

    return res;
}

int main(int argv, char *argc[])
{
    // char* getcwd(char *buf, size_t size)
    // char* getcwd(char *buf)

    char buffer[1024] = "/proc/";
    // char *path = getcwd(buffer, sizeof(buffer));
    char mode_str[10];

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
        lstat(buffer, &fileInfo);
        int statmod = fileInfo.st_mode & 0777;
        printf("%c %s%s%s  %s\n", dirType(fileInfo.st_mode), rwx[(fileInfo.st_mode >> 6) & 7],
               rwx[(fileInfo.st_mode >> 3) & 7], rwx[fileInfo.st_mode & 7], buffer);
    }

    if (closedir(d) == ERR)
        exit(ERR);

    return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/dir.h>
#include "colors.h"

#define ERR -1

static const char *rwx[] = {
    "---", "--x", "-w-", "-wx",
    "r--", "r-x", "rw-", "rwx"};

const char *userperms(mode_t m)
{
    return rwx[(m >> 6) & 7];
}

const char *grpperms(mode_t m)
{
    return rwx[(m >> 3) & 7];
}

const char *otherperms(mode_t m)
{
    return rwx[m & 7];
}

char dirType(mode_t mode)
{
    char res = ' ';

    if (S_ISDIR(mode))
    {
        printf(RED_C);
        res = 'd';
    }
    else if (S_ISLNK(mode))
    {
        printf(BLUE_C);
        res = 'l';
    }
    else
    {
        printf(WHITE_C);
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

    struct dirent **files;
    struct stat fileInfo;
    int n;
    DIR *d = opendir(buffer);

    if (d == 0)
    {
        perror("Path");
        exit(1);
    }

    n = scandir(buffer, &files, 0, alphasort);

    if (n == ERR)
    {
        perror("scandir");
        exit(ERR);
    }

    for (int i = 0; i < n; i++, strcpy(buffer, "/proc/"))
    {
        strcat(buffer, files[i]->d_name);
        lstat(buffer, &fileInfo);
        printf("%c%s%s%s %ld %s\n", dirType(fileInfo.st_mode), userperms(fileInfo.st_mode),
               grpperms(fileInfo.st_mode), otherperms(fileInfo.st_mode), fileInfo.st_nlink, buffer);
        printf(RESET_C);
    }

    if (closedir(d) == ERR)
        exit(ERR);

    return 0;
}
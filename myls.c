#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/dir.h>
#include <time.h>
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

    int a = 0;
    int r = 0;

    if (argv > 1)
    {
        for (int i = 1; i < argv; i++)
        {
            if (argc[i][0] == '-')
            {
                if (strchr(argc[i], 'a') != NULL)
                {
                    a++;
                }
                if (strchr(argc[i], 'r') != NULL || strchr(argc[i], 'R') != NULL)
                {
                    r++;
                }
            }
        }
    }

    char buffer[1024] = "./";
    char path[1024];
    strcpy(path, buffer);
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

    for (int i = 0; i < n; i++, strcpy(path, buffer))
    {
        strcat(path, files[i]->d_name);
        lstat(path, &fileInfo);
        // permissions
        printf("%c%s%s%s %ld", dirType(fileInfo.st_mode), userperms(fileInfo.st_mode),
               grpperms(fileInfo.st_mode), otherperms(fileInfo.st_mode), fileInfo.st_nlink);
        // date
        printf(" %7ld %.12s ", fileInfo.st_size, ctime(&fileInfo.st_mtime) + 4);
        // fichier
        printf("%s\n", path);
        printf(RESET_C);
    }

    if (closedir(d) == ERR)
        exit(ERR);

    return 0;
}
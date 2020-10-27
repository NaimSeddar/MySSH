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

#define MAX_DIR 256

static const char *perms[] = {
    "---", "--x", "-w-", "-wx",
    "r--", "r-x", "rw-", "rwx"};

const char *userperms(mode_t m)
{
    return perms[(m >> 6) & 7];
}

const char *grpperms(mode_t m)
{
    return perms[(m >> 3) & 7];
}

const char *otherperms(mode_t m)
{
    return perms[m & 7];
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

void print_perms(struct stat s)
{
    mode_t m = s.st_mode;
    // permissions
    printf("%c%s%s%s %ld", dirType(m), userperms(m), grpperms(m), otherperms(m), s.st_nlink);
}

void print_date(struct stat s)
{
    // date
    printf(" %7ld %.12s ", s.st_size, ctime(&s.st_mtime) + 4);
}

void print_line(struct stat s, char *name)
{
    print_perms(s);
    print_date(s);
    printf("%s\n\n", name);
    printf(RESET_C);
}

void myls(char *path, int a, int r)
{
    char fpath[1024];
    strcpy(fpath, path);

    struct dirent **files;
    struct stat fileInfo;
    int n;

    n = scandir(path, &files, 0, alphasort);

    if (n == ERR)
    {
        perror("scandir");
        exit(ERR);
    }

    for (int i = 0; i < n; i++)
    {
        if (!a && files[i]->d_name[0] == '.')
            continue;
        strcpy(fpath, path);
        strcat(fpath, files[i]->d_name);
        lstat(fpath, &fileInfo);

        print_line(fileInfo, fpath);
    }
}

int main(int argv, char *argc[])
{

    pid_t p;
    int n, status;
    int a = 0;
    int r = 0;
    char *dir[MAX_DIR];
    int nb_dir = 0;

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
            else
            {
                dir[nb_dir++] = argc[i];
            }
        }

        for (int i = 0; i < nb_dir; i++)
        {
            printf("%s\n", dir[i]);
        }
    }

    return 0;
}
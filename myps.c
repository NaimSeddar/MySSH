#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <string.h>

#define ERR -1

int main()
{

    struct dirent **proc;
    struct stat procInfo;
    struct passwd *p;
    int n;
    char ppath[1024] = "/proc/";

    n = scandir(ppath, &proc, NULL, alphasort);

    if (n == ERR)
    {
        perror("scandir");
        exit(ERR);
    }

    for (int i = 2; i < n && isdigit(*proc[i]->d_name); i++, strcpy(ppath, "/proc/"))
    {
        strcat(ppath, proc[i]->d_name);
        lstat(ppath, &procInfo);
        p = getpwuid(procInfo.st_uid);
        printf("User: %-10s PID: %5s Dir: %s\n",
               p->pw_name, proc[i]->d_name, ppath);
    }

    printf("Un 2 roues en 4 roues\n");
    return 0;
}
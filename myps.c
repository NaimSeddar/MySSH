#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <string.h>

#define ERR -1

void getstatus(char *pid)
{
    char filename[1024];
    char cmd[1024];
    int useless;
    char state;
    sprintf(filename, "/proc/%s/stat", pid);

    FILE *f = fopen(filename, "r");

    fscanf(f, "%d %s %c", &useless, cmd, &state);

    printf("cmd: %-10s state: %c\n", cmd, state);

    fclose(f);
}

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
        printf("User: %-10s PID: %5s ",
               p->pw_name, proc[i]->d_name);
        getstatus(proc[i]->d_name);
    }

    return 0;
}
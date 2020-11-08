#include "myps.h"

int getmemtotal()
{
    char buffer[1024];
    int memtotal;

    FILE *f = fopen("/proc/meminfo", "r");

    fscanf(f, "MemTotal: %d kb", &memtotal);

    close(f);

    return memtotal;
}

void getstatus(char *pid)
{
    char filename[1024];
    char cmd[1024];
    int useless;
    char state[8];
    sprintf(filename, "/proc/%s/stat", pid);

    FILE *f = fopen(filename, "r");

    fscanf(f, "%d %s %s", &useless, cmd, state);

    printf("status: %s ", state);

    fclose(f);
}

void getcmd(char *pid)
{
    char filename[1024];
    char cmd[1024];
    char c;
    int i = 0;

    sprintf(filename, "/proc/%s/cmdline", pid);
    int f = open(filename, O_RDONLY);

    // La commande et ses arguments sont séparés par '\0'
    // On doit donc parcourir la chaine manuellement
    while (read(f, &c, 1) > 0)
    {
        cmd[i] = (c == '\0' ? ' ' : c);
        i++;
    }
    cmd[i] = '\0';

    printf("cmd: %s \n", cmd);

    close(f);
}

void getstart(struct stat s)
{
    printf("start: %.5s ", ctime(&s.st_ctime) + 11);
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

    /*for (int i = 2; i < n && isdigit(*proc[i]->d_name); i++, strcpy(ppath, "/proc/"))
    {
        strcat(ppath, proc[i]->d_name);
        lstat(ppath, &procInfo);
        p = getpwuid(procInfo.st_uid);
        printf("User: %-10s PID: %5s ",
               p->pw_name, proc[i]->d_name);
        getstatus(proc[i]->d_name);
        getcmd(proc[i]->d_name);
        getstart(procInfo);
        printf("\n");
    }*/

    printf("Memtotal: %d\n", getmemtotal());
    getcmd("7");

    return 0;
}
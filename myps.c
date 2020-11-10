#include "myps.h"

int getmemtotal()
{
    int memtotal;

    FILE *f = fopen("/proc/meminfo", "r");

    fscanf(f, "MemTotal: %d kb", &memtotal);

    fclose(f);

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

void getcmd(char *pid, proc *p)
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

    p->command = (char *)malloc(sizeof(char) * i);
    strncpy(p->command, cmd, i);

    // printf("cmd: %s \n", cmd);

    close(f);
}

void getstart(struct stat s)
{
    printf("start: %.5s ", ctime(&s.st_ctime) + 11);
}

void print_proc(proc *p)
{
    printf("user: %-16s; pid: %8s; cmd: %s\n",
           p->user, p->pid, p->command);
}

int main()
{

    struct dirent **dir;
    struct stat dirInfo;
    struct passwd *pswd;
    int n;
    char ppath[1024] = "/proc/";
    proc *p;

    n = scandir(ppath, &dir, NULL, alphasort);

    if (n == ERR)
    {
        perror("scandir");
        exit(ERR);
    }

    for (int i = 2; i < n && isdigit(*dir[i]->d_name); i++, strcpy(ppath, "/proc/"))
    {
        p = (proc *)malloc(sizeof(proc));
        p->pid = dir[i]->d_name;
        strcat(ppath, p->pid);
        lstat(ppath, &dirInfo);
        pswd = getpwuid(dirInfo.st_uid);
        p->user = pswd->pw_name;
        getcmd(p->pid, p);

        /*strcat(ppath, dir[i]->d_name);
        lstat(ppath, &procInfo);
        pswd = getpwuid(procInfo.st_uid);
        printf("User: %-10s PID: %5s ",
               p->pw_name, proc[i]->d_name);
        getstatus(proc[i]->d_name);
        getcmd(proc[i]->d_name);
        getstart(procInfo);
        printf("\n");*/
        // printf("command: %s\n", p->command);
        print_proc(p);
    }

    // printf("Memtotal: %d\n", getmemtotal());

    return 0;
}
#include "myps.h"
#include "utils.h"

int getmemtotal()
{
    int memtotal;

    FILE *f = fopen("/proc/meminfo", "r");

    fscanf(f, "MemTotal: %d kb", &memtotal);

    fclose(f);

    return memtotal;
}

float getmem(proc *p)
{
    return
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

void getstart(struct stat s, proc *p)
{
    // p->start = (char *)malloc(sizeof(char) * 5);
    p->start = (ctime(&s.st_ctime) + 11);
    // printf("start: %.5s ", ctime(&s.st_ctime) + 11);
}

void parse_status(proc *p)
{
    char filename[1024];
    char *buffer;
    char *seek;
    int rss = 0;
    int vsz = 0;

    sprintf(filename, "/proc/%s/status", p->pid);

    buffer = file_to_string(filename);

    /* if we find the substring "VmRSS", then we cand extract his value */
    if ((seek = strstr(buffer, "VmRSS")))
        sscanf(seek, "VmRSS:\t%d ", &rss);
    p->rss = rss;

    if ((seek = strstr(buffer, "VmSize")))
        sscanf(seek, "VmSize:\t%d ", &vsz);
    p->vsz = vsz;
}

void print_proc(proc *p)
{
    printf("%-8s %8s %8d %8d %.5s %s\n",
           p->user, p->pid, p->rss, p->vsz, p->start, p->command);
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

    /* print command header */
    printf("%-8s %8s %8s %8s %.5s %s\n",
           "user", "pid", "rss", "vsz", "start", "command");

    /* i starting at 2 in order to skip '.' and '..' */
    for (int i = 2; i < n && isdigit(*dir[i]->d_name); i++, strcpy(ppath, "/proc/"))
    {
        p = (proc *)malloc(sizeof(proc));
        p->pid = dir[i]->d_name;

        strcat(ppath, p->pid);
        lstat(ppath, &dirInfo);
        pswd = getpwuid(dirInfo.st_uid);

        p->user = pswd->pw_name;

        getcmd(p->pid, p);
        parse_status(p);
        getstart(dirInfo, p);

        print_proc(p);

        free(dir[i]);
        free(p);
    }
    free(dir);

    return 0;
}
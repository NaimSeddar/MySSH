#include "myps.h"
#include "utils.h"
#include "colors.h"

float getuptime()
{
    float uptime;

    FILE *f = fopen("/proc/uptime", "r");

    fscanf(f, "%f", &uptime);

    fclose(f);

    return uptime;
}

int getmemtotal()
{
    int memtotal;

    FILE *f = fopen("/proc/meminfo", "r");

    fscanf(f, "MemTotal: %d kb", &memtotal);

    fclose(f);

    return memtotal;
}

void getmem(proc *p)
{
    p->mem = ((float)p->rss / (float)getmemtotal()) * 100;
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

    p->command = (char *)malloc(sizeof(char) * i);
    strncpy(p->command, cmd, i - 1);

    close(f);
}

void getstart(struct stat s, proc *p)
{
    p->start = (ctime(&s.st_ctime) + 11);
}

void parse_status(proc *p)
{
    char filename[1024];
    char *buffer;
    char *seek;
    int rss = 0;
    int vsz = 0;
    int threads = 0;
    int lock = 0;

    sprintf(filename, "/proc/%s/status", p->pid);

    buffer = file_to_string(filename);

    /* if we find the substring "VmRSS", then we cand extract his value */
    if ((seek = strstr(buffer, "VmRSS")))
        sscanf(seek, "VmRSS:\t%d ", &rss);
    p->rss = rss;

    if ((seek = strstr(buffer, "VmSize")))
        sscanf(seek, "VmSize:\t%d ", &vsz);
    p->vsz = vsz;

    if ((seek = strstr(buffer, "Threads")))
        sscanf(seek, "Threads:\t%d ", &threads);
    p->threads = threads;

    if ((seek = strstr(buffer, "VmLck")))
        sscanf(seek, "VmLck:\t%d ", &threads);
    p->lock = lock;
}

void parse_stat(proc *p)
{
    char filename[1024];
    sprintf(filename, "/proc/%s/stat", p->pid);
    char *buffer = file_to_string(filename);
    char **values = str_split(buffer, ' ');
    size_t tmp;
    int stat_cpt;

    /* Getting CPU %*/
    long hertz = sysconf(_SC_CLK_TCK);
    int utime = atof(values[13]);
    int stime = atof(values[14]);
    int cutime = atof(values[15]);
    int cstime = atof(values[16]);
    int start = atof(values[21]);
    float s = getuptime() - (start / hertz);
    int sum = utime + stime + cutime + cstime;
    p->time_cumul = s;
    p->cpu = ((sum / hertz) / s) * 100;

    /* Getting TTY */
    p->tty = (char *)malloc(sizeof(char) * 256);
    sprintf(buffer, "/proc/%s/fd/0", p->pid);
    if ((tmp = readlink(buffer, p->tty, (size_t)1024)) == -1 || strncmp(p->tty, "/dev/null", 9) == 0)
    {
        free(p->tty);
        p->tty = "?";
    }
    else
    {
        p->tty[tmp] = '\0';
        p->tty += 5;
    }

    /* Getting status */
    p->stat = (char *)malloc(sizeof(char) * 8);
    p->stat = values[2];
    stat_cpt = 1;

    /* 
     * Check if high prioty ('<') or not ('N')
     * Nice value range: -20(high) to 19(low)
     */
    if (values[18][0] == '-')
    {
        p->stat[stat_cpt++] = '<';
    }
    else if (values[18][0] == '1')
    {
        p->stat[stat_cpt++] = 'N';
    }

    /* Check if it has pages locked into memory */
    if (p->lock)
    {
        p->stat[stat_cpt++] = 'L';
    }

    /* Check if it's a session leader */
    if (strncmp(p->pid, values[5], strlen(p->pid)) == 0)
    {
        p->stat[stat_cpt++] = 's';
    }

    /* Check if it's multi threaded */
    if (p->threads > 1)
    {
        p->stat[stat_cpt++] = 'l';
    }

    /* Check if it's in the foreground process group */
    if (strncmp(p->pid, values[7], strlen(p->pid)) == 0)
    {
        p->stat[stat_cpt++] = '+';
    }

    p->stat[stat_cpt] = '\0';

    free(buffer);
}

void getcolor(proc *p)
{
    switch (p->stat[0])
    {
    case 'S':
        printf(BLUE_C);
        break;

    case 'R':
        printf(GREEN_C);
        break;

    case 'Z':
        printf(RED_C);
        break;

    case 'D':
        printf(WHITE_C);
        break;

    case 'T':
        grayBG();
        break;

    default:
        break;
    }
}

void print_proc(proc *p)
{
    getcolor(p);
    printf("%-8s %8s %8.1f %8.1f %8d %8d %8s %8s %.5s %.5s %s %s\n",
           p->user, p->pid, p->cpu, p->mem, p->rss, p->vsz, p->tty, p->stat, p->start, p->time, p->command, RESET_C);
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
    printf("%-8s %8s %8s %8s %8s %8s %8s %8s %.5s %.4s %s\n",
           "USER", "PID", "%CPU", "%MEM", "RSS", "VSZ", "TTY", "STAT", "START", "TIME", "COMMAND");

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
        getstart(dirInfo, p);
        parse_status(p);
        getmem(p);
        parse_stat(p);
        double t = p->time_cumul;
        int secs = t / sysconf(_SC_CLK_TCK);
        int mins = secs == 0 ? 0 : 60 / (t - (double)secs);
        int hours = mins == 0 ? 0 : 60 / (t - (double)mins);
        sprintf(p->time, "%02d:%02d", mins, secs);
        // p->time = (ctime(&dirInfo.st_mtime) + 11);

        print_proc(p);

        free(dir[i]);
        // free(p->tty);
        free(p);
    }
    free(dir);

    return 0;
}
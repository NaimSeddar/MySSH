#include "myls.h"

char *perms[] = {
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

char dirType(mode_t m)
{
    char res = ' ';

    if (S_ISDIR(m))
    {
        printf(RED_C);
        res = 'd';
    }
    else if (S_ISLNK(m))
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

void print_dirHeader(char *dir)
{
    whiteBG();
    printf(RED_C "%s" RESET_C "\n", dir);
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
    printf("%s\n", name);
    printf(RESET_C);
}

void myls(char *path, int a, int r)
{
    char fpath[1024];
    strcpy(fpath, path);

    char *dir[MAX_DIR];
    int nb_dir = 0;

    struct dirent **files;
    struct stat fileInfo;
    int n;

    n = scandir(path, &files, 0, alphasort);

    if (n == ERR)
    {
        perror("scandir");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        if (!a && files[i]->d_name[0] == '.')
        {
            continue;
        }

        strcpy(fpath, path);
        if (fpath[strlen(fpath) - 1] != '/')
        {
            strncat(fpath, "/", 1);
        }
        strcat(fpath, files[i]->d_name);
        lstat(fpath, &fileInfo);
        print_line(fileInfo, fpath);
        if (S_ISDIR(fileInfo.st_mode) && r && ((a && i > 1) || !a))
        {
            // printf(BLUE_C "%s\n" RESET_C, files[i]->d_name);
            dir[nb_dir] = (char *)malloc(sizeof(fpath));
            strcpy(dir[nb_dir], fpath);
            nb_dir++;
            // dir[nb_dir++] = fpath;
        }
        free(files[i]);
    }
    free(files);
    for (int i = 0; i < nb_dir; i++)
    {
        printf("\n");
        print_dirHeader(dir[i]);
        myls(dir[i], a, r);
        free(dir[i]);
    }
}

int main(int argv, char *argc[])
{

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

        if (nb_dir == 1)
        {
            myls(dir[0], a, r);
        }
        else
        {
            for (int i = 0; i < nb_dir; i++)
            {
                print_dirHeader(dir[i]);
                myls(dir[i], a, r);
                printf("\n");
            }
        }
    }

    return 0;
}
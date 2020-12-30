/**
 * Auteur:                Seddar Naïm
 * Création:              24/10/2020 20:59:37
 * Dernière modification: 30/12/2020 17:15:05
 * Master 1 Informatique
 */

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include "../includes/mysh.h"

pid_t cmd_pid = -1;
pid_t prev_fg_proc = -1;

int pcode = 0;
int nb_jobs = 0;
int prev_pcode = -1;

char prev_cmd[4096];

struct myjob jobs[256];

void ctrlc(int sig)
{
    if (cmd_pid == -1)
    {
        char resp = '\0';

        printf("\nDo you really want to quit ? [Y/n] \n");

        do
        {
            resp = getchar();
        } while (resp == '\n' || resp == '\0');

        if (resp == 'y' || resp == 'Y')
        {
            printf("Farewell...\n");
            kill(0, SIGKILL);
            exit(0);
        }
        else
        {
            printprompt(pcode);
            return;
        }
    }
    else
    {
        kill(cmd_pid, SIGKILL);
        cmd_pid = -1;
    }
}

void ctrlz(int sig)
{
    if (cmd_pid != -1)
    {
        jobs[nb_jobs].show = 1;
        jobs[nb_jobs].job_id = nb_jobs;
        jobs[nb_jobs].pid = cmd_pid;
        jobs[nb_jobs].etat = "Stoppé";
        memcpy(jobs[nb_jobs].command, prev_cmd, strlen(prev_cmd));
        printf("\n[%d] %s\n", jobs[nb_jobs].job_id, jobs[nb_jobs].command);
        nb_jobs++;
        killpg(cmd_pid, SIGSTOP);
    }
    else
    {
        printprompt(pcode);
        return;
    }
}

void check_jobs()
{
    int status;
    pid_t p;

    if (nb_jobs == 0)
        return;

    for (int i = 0; i < nb_jobs; i++)
    {
        if (jobs[i].show)
        {
            p = waitpid(jobs[i].pid, &status, WNOHANG);

            if (p == -1)
                jobs[i].show = 0;
        }
    }
}

int run_it_in_bg(char **fields)
{
    for (int i = 0; *(fields + i); i++)
    {
        if (strlen(*(fields + i)) == 1 && *(fields + i)[0] == '&')
        {
            *(fields + i) = NULL;
            return 1;
        }
    }

    return 0;
}

int systemV2(char *command)
{
    int status;
    char **commands = str_split(command, ' ');

    if (*commands == NULL)
        return 0;

    search_replace_var(commands);

    int bg = run_it_in_bg(commands);

    int b_in = builtin_parser(commands);
    if (b_in != -2)
    {
        prev_fg_proc = getpid();
        prev_pcode = b_in;

        return b_in;
    }

    pid_t pid = fork();
    cmd_pid = pid;

    if (pid == ERR)
    {
        perror("Fork");
        return ERR;
    }

    if (!pid)
    {
        if (bg)
        {
            setpgid(0, 0);
        }

        if (commands[1] == NULL)
        {

            if (execvp(*commands, commands) == -1)
            {
                fprintf(stderr, "%s: Unknown command\n", *commands);
                perror("");
            }
            else
            {
                while (*commands != NULL)
                {
                    free(*(commands++));
                }
            }
        }
        else
        {
            glob_t glob_buffer = {0};
            int r;
            glob_buffer.gl_offs = 1;

            r = glob(commands[1], GLOB_DOOFFS | GLOB_NOCHECK, NULL, &glob_buffer);
            if (r != 0)
            {
                perror("glob");
                exit(EXIT_FAILURE);
            }

            for (int i = 2; *(commands + i); i++)
            {
                if ((r = glob(*(commands + i), GLOB_DOOFFS | GLOB_NOCHECK | GLOB_APPEND, NULL, &glob_buffer)) != 0)
                {
                    perror("glob");
                    exit(EXIT_FAILURE);
                }
            }
            glob_buffer.gl_pathv[0] = *commands;
            if (execvp(glob_buffer.gl_pathv[0], &glob_buffer.gl_pathv[0]) == -1)
            {
                fprintf(stderr, "%s: Unknown command\n", *commands);
                perror("");
            }
            else
            {
                while (*commands != NULL)
                {
                    free(*(commands++));
                }
            }
        }

        free(commands);

        exit(FAILED_EXEC);
    }

    if (!bg)
    {

        prev_fg_proc = cmd_pid;
        if (waitpid(-1, &status, WUNTRACED) == ERR)
        {
            perror("wait");
            return ERR;
        }
    }
    else
    {
        jobs[nb_jobs].show = 1;
        jobs[nb_jobs].job_id = nb_jobs;
        jobs[nb_jobs].pid = cmd_pid;
        jobs[nb_jobs].etat = "En cours d'exécution";
        memcpy(jobs[nb_jobs].command, command, strlen(command) + 1);
        printf("[%d] %d %s %s\n", jobs[nb_jobs].job_id, jobs[nb_jobs].pid, jobs[nb_jobs].etat, jobs[nb_jobs].command);
        nb_jobs++;
    }

    if (WIFEXITED(status))
    {
        prev_pcode = WEXITSTATUS(status);
        return prev_pcode;
    }

    prev_pcode = ERR;

    return ERR;
}

int parser(char *command)
{
    int res;
    char **commands = str_split(command, ';');

    for (int i = 0; *(commands + i); i++)
    {
        printf("(%s)\n", *(commands + i));

        /* Check if there's a || */
        if (strstr(*(commands + i), "||") != NULL)
        {
            res = or_op(*(commands + i));
        }
        /* Check if there's a && */
        else if (strstr(*(commands + i), "&&") != NULL)
        {
            res = and_op(*(commands + i));
        }
        /* Check if there's a 2>> */
        else if (strstr(*(commands + i), "2>>") != NULL)
        {
            res = stderr_to_fic(*(commands + i), O_CREAT | O_WRONLY | O_APPEND);
        }
        /* Check if there's a 2> */
        else if (strstr(*(commands + i), "2>") != NULL)
        {
            res = stderr_to_fic(*(commands + i), O_CREAT | O_WRONLY | O_TRUNC);
        }
        /* Check if there's a >>& */
        else if (strstr(*(commands + i), ">>&") != NULL)
        {
            res = stderr_and_stdout(*(commands + i), O_CREAT | O_WRONLY | O_APPEND);
        }
        /* Check if there's a >> */
        else if (strstr(*(commands + i), ">>") != NULL)
        {
            res = stdout_to_fic(*(commands + i), O_CREAT | O_WRONLY | O_APPEND);
        }
        /* Check if there's a >& */
        else if (strstr(*(commands + i), ">&") != NULL)
        {
            res = stderr_and_stdout(*(commands + i), O_CREAT | O_WRONLY | O_TRUNC);
        }
        /* Check if there's a > */
        else if (strstr(*(commands + i), ">") != NULL)
        {
            res = stdout_to_fic(*(commands + i), O_CREAT | O_WRONLY | O_TRUNC);
        }
        /* Check if there's a < */
        else if (strstr(*(commands + i), "<") != NULL)
        {
            res = fic_to_stdin(*(commands + i));
        }
        /* Check if there's a pipe */
        else if (strchr(*(commands + i), '|') != NULL)
        {
            res = pipeline(*(commands + i));
        }
        else
        {
            printf("Exec -> (%s)\n", *(commands + i));
            res = systemV2(*(commands + i));
        }
    }
    free(commands);
    return res;
}

int and_op(char *command)
{
    int res = 0;
    char **buffer = str_splitv2(command, "&&");

    for (int i = 0; *(buffer + i); i++)
    {
        res = parser(*(buffer + i));

        if (res)
            return res;
    }

    return res;
}

int or_op(char *command)
{
    int res = 0;
    char **buffer = str_splitv2(command, "||");

    for (int i = 0; *(buffer + i); i++)
    {
        res = parser(*(buffer + i));

        if (!res)
            return res;
    }

    return res;
}

int pipeline(char *command)
{
    int p[2];
    pid_t pid;
    int fd_in = 0;
    int i = 0;
    int status;
    char **cmds = str_split(command, '|');

    while (cmds[i])
    {
        pipe(p);

        if ((pid = fork()) == -1)
        {
            exit(1);
        }
        else if (pid == 0)
        {
            dup2(fd_in, 0);

            if (cmds[i + 1])
                dup2(p[1], 1);

            close(p[0]);
            exit(parser(cmds[i]));
        }
        else
        {
            if (wait(&status) == ERR)
            {
                return ERR;
            }
            close(p[1]);
            fd_in = p[0];
            i++;
        }
    }

    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }

    return ERR;
}

void search_replace_var(char **commands)
{
    for (int i = 0; *(commands + i); i++)
    {
        if (*(commands + i)[0] == '$')
        {
            *(commands + i) = getenv(++(*(commands + i)));
        }
    }
}

void printprompt(int pcode)
{
    char currpath[1024];
    char *username;
    int len;

    if (getcwd(currpath, sizeof(currpath)) == NULL)
    {
        perror("getcwd error");
        exit(1);
    }

    len = strlen(currpath);
    currpath[len] = '$';
    currpath[len + 1] = ' ';
    currpath[len + 2] = '\0';

    username = getenv("USER");

    if (username != NULL)
    {
        printf(U_RED_C "%s:" RESET_C, username);
    }

    printf(GREEN_C "%s" RESET_C, currpath);
    fflush(stdout);
}

void mysh()
{
    signal(SIGINT, ctrlc);
    signal(SIGTSTP, ctrlz);

    char *buffer;

    for (;;)
    {
        check_jobs();
        cmd_pid = -1;

        printprompt(pcode);

        buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
        memset(buffer, '\0', BUFFER_SIZE);

        if (read(STDIN_FILENO, buffer, 1024) == ERR)
            perror("Read"), exit(2);

        /* replace '\n' by '\0' */
        buffer[strlen(buffer) - 1] = '\0';

        memcpy(prev_cmd, buffer, strlen(buffer));

        pcode = parser(buffer);

        free(buffer);
    }
}

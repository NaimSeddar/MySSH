/**
 * Auteur:                Seddar Naïm
 * Création:              24/10/2020 20:59:37
 * Dernière modification: 28/12/2020 21:00:26
 * Master 1 Informatique
 */

#define _DEFAULT_SOURCE
#include "../includes/mysh.h"

/*void ctrlz(int sig)
{
    printf("pid to stop : %d (from %d) %d\n", getpid(), getppid(), cmd_pid);
    kill((cmd_pid == -1 ? getpid() : cmd_pid), SIGSTOP);
}*/

pid_t cmd_pid = -1;

int run_in_bg(char **fields)
{
    for (int i = 0; *(fields + i); i++)
    {
        if (strlen(*(fields + i)) == 1 && *(fields + i)[0] == '&')
        {
            *(fields + i) = NULL;
            // printf("go en bg\n");
            return 1;
        }
    }

    return 0;
}

/**
 * A better system() saw in class.
 * A lighter version based on execvp. 
 * @param command
 * */
int systemV2(char *command)
{
    int status;
    char **commands = str_split(command, ' ');

    if (*commands == NULL)
        return 0;

    search_replace_var(commands);

    // int bg = run_in_bg(commands);

    int b_in = builtin_parser(commands);
    if (!b_in)
        return b_in;

    pid_t pid = fork();
    cmd_pid = pid;

    if (pid == ERR)
    {
        perror("Fork");
        return ERR;
    }

    if (!pid)
    {
        // if (bg)
        // {
        //     setpgid(0, 0);
        // }

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

    // if (!bg)
    // {
    if (wait(&status) == ERR)
    {
        perror("wait");
        return ERR;
    }
    // }

    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }

    return ERR;
}

int parser(char *command)
{
    int res;
    char **commands = str_split(command, ';');
    // printf("Entrer parser (%s)\n", *commands);
    for (int i = 0; *(commands + i); i++)
    {
        // printf("blabla haut\n");
        // printf("Loop parser (%d : %s)\n", i, *(commands + i));

        /* Check if there's a pipe*/
        if (strstr(*(commands + i), "||") != NULL)
        {
            printf("Lance un OU sur: (%s)\n", *(commands + i));
            res = or_op(*(commands + i));
        }
        else if (strstr(*(commands + i), "&&") != NULL)
        {
            // printf("Lance un ET sur: (%s)\n", *(commands + i));
            res = and_op(*(commands + i));
        }
        /* Check if there's a pipe*/
        else if (strchr(*(commands + i), '|') != NULL)
        {
            printf("Lance pipeline sur: (%s)\n", *(commands + i));
            res = pipeline(*(commands + i));
        }
        else if (strstr(*(commands + i), "2>>") != NULL)
        {
            printf("Redirection 2>> sur: (%s)\n", *(commands + i));
            res = stderr_to_fic(*(commands + i), O_CREAT | O_WRONLY | O_APPEND);
        }
        else if (strstr(*(commands + i), "2>") != NULL)
        {
            printf("Redirection 2> sur: (%s)\n", *(commands + i));
            res = stderr_to_fic(*(commands + i), O_CREAT | O_WRONLY | O_TRUNC);
        }
        else if (strstr(*(commands + i), ">>&") != NULL)
        {
            printf("Redirection >>& sur: (%s)\n", *(commands + i));
            res = stderr_and_stdout(*(commands + i), O_CREAT | O_WRONLY | O_APPEND);
        }
        else if (strstr(*(commands + i), ">>") != NULL)
        {
            printf("Redirection >> sur: (%s)\n", *(commands + i));
            res = stdout_to_fic(*(commands + i), O_CREAT | O_WRONLY | O_APPEND);
        }
        else if (strstr(*(commands + i), ">&") != NULL)
        {
            printf("Redirection >& sur: (%s)\n", *(commands + i));
            res = stderr_and_stdout(*(commands + i), O_CREAT | O_WRONLY | O_TRUNC);
        }
        else if (strstr(*(commands + i), ">") != NULL)
        {
            printf("Redirection > sur: (%s)\n", *(commands + i));
            res = stdout_to_fic(*(commands + i), O_CREAT | O_WRONLY | O_TRUNC);
        }
        else if (strstr(*(commands + i), "<") != NULL)
        {
            printf("Redirection < sur: (%s)\n", *(commands + i));
            res = fic_to_stdin(*(commands + i));
        }
        else
        {
            printf("Lance exec sur: (%s)\n", *(commands + i));
            res = systemV2(*(commands + i));
        }
        // printf("blabla bas\n");
        // free(commands + i);
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
        printf("Handle this (%s)\n", *(buffer + i));
        res = parser(*(buffer + i));
        if (res)
            return res;
        // free(buffer + i);
    }
    // free(buffer);

    return res;
}

int or_op(char *command)
{
    int res = 0;
    char **buffer = str_splitv2(command, "||");

    for (int i = 0; *(buffer + i); i++)
    {
        printf("Handle this (%s)\n", *(buffer + i));
        res = parser(*(buffer + i));
        if (!res)
            return res;
        // free(buffer + i);
    }
    // free(buffer);

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
            printf("exec in pipe: %s\n", cmds[i]);
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
            // fflush(stdout);
            printf("var: %s\n", getenv("USER"));
        }
    }
}

void printprompt()
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

    // username = getpwuid(getuid())->pw_name;
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

    // signal(SIGTSTP, ctrlz);

    char *buffer;

    for (;;)
    {
        cmd_pid = -1;
        printprompt();
        // printf("%s$ ", currpath);

        buffer = malloc(BUFFER_SIZE * sizeof(char));
        memset(buffer, '\0', BUFFER_SIZE);

        if (read(STDIN_FILENO, buffer, 1024) == ERR)
            perror("Read"), exit(2);

        // replace '\n' by '\0'
        buffer[strlen(buffer) - 1] = '\0';

        /*if (strncmp(buffer, "exit", 4) == 0)
        {
            // printf("strcmp: %d\n", strcmp(buffer, "exit"));
            free(buffer);
            break;
        }*/

        printf(YELLOW_C "[%d]" RESET_C, parser(buffer));

        // free(commands);

        free(buffer);
    }
}

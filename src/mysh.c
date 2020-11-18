#include "../includes/mysh.h"

/**
 * A better system(), saw in class.
 * A lighter version base on execvp. 
 * @param command
 * */
int systemV2(char *command)
{
    int status;
    char **commands = str_split(command, ' ');
    pid_t pid = fork();

    // for (int i = 0; *(commands + i); i++)
    // {
    //     printf("<%s> ", *(commands + i));
    // }
    // printf("\n");

    if (pid == ERR)
    {
        perror("Fork");
        return ERR;
    }

    if (!pid)
    {
        // printf("To exec: %s\n", command);
        // execl("/bin/sh", "sh", "-c", command, (const char *)0);
        execvp(*commands, commands);

        // while (*(commands++))
        // {
        //     free(*commands);
        // }
        // free(commands);
        perror("execvp failed");
        exit(FAILED_EXEC);
    }

    if (wait(&status) == ERR)
    {
        perror("wait");
        return ERR;
    }

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
    printf("Entrer parser (%s)\n", *commands);
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
        else
        {
            // printf("Lance exec sur: (%s)\n", *(commands + i));
            res = systemV2(command);
        }
        // printf("blabla bas\n");
        // free(commands + i);
    }
    // free(commands);
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

void printprompt()
{
    char currpath[1024];
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

    // writein(GREEN_C);
    // if (write(STDOUT_FILENO, currpath, strlen(currpath)) == ERR)
    //     perror("Write"), exit(1);
    // writein(RESET_C);

    printf(GREEN_C "%s" RESET_C, currpath);
    fflush(stdout);
}

int main(int argv, char *argc[])
{
    if (argv > 1)
    {
        printf("Aucun argument n'est requis pour '%s'\n", argc[1]);
        exit(1);
    }

    char *buffer;

    for (;;)
    {
        printprompt();
        // printf("%s$ ", currpath);

        buffer = malloc(BUFFER_SIZE * sizeof(char));
        memset(buffer, '\0', BUFFER_SIZE);

        if (read(STDIN_FILENO, buffer, 1024) == ERR)
            perror("Read"), exit(2);

        // replace '\n' by '\0'
        buffer[strlen(buffer) - 1] = '\0';

        if (strncmp(buffer, "exit", 4) == 0)
        {
            // printf("strcmp: %d\n", strcmp(buffer, "exit"));
            free(buffer);
            break;
        }
        else
        {

            // if "cd" --> chdir(arg)
            // buffer = strtok(buffer, ';');
            /*for (int i = 0; *(commands + i); i++)
            {
                // printf("<%s>\n", str_split(*(commands + i), ' ')[0]);
                printf("status: %d\n", systemV2(str_split(*(commands + i), ' ')));
                free(*(commands + i));
            }*/
            printf(YELLOW_C "[%d]" RESET_C, parser(buffer));

            // free(commands);
        }
        free(buffer);
    }

    return 0;
}
#include "../includes/mysh.h"

pid_t cmd_pid = -1;

/* TODO
 *  Demander confirmation, et tuer tout les enfants en fond.
 */
void ctrlc(int sig)
{
    printf("pid to kill : %d (from %d) %d\n", getpid(), getppid(), cmd_pid);
    if (cmd_pid == -1)
    {
        printf("kill main\n");
        kill(getpid(), SIGKILL);
    }
    else
    {
        kill(cmd_pid, SIGKILL);
    }
    cmd_pid = -1;
}

/*void ctrlz(int sig)
{
    printf("pid to stop : %d (from %d) %d\n", getpid(), getppid(), cmd_pid);
    kill((cmd_pid == -1 ? getpid() : cmd_pid), SIGSTOP);
}*/

/**
 * A better system(), saw in class.
 * A lighter version base on execvp. 
 * @param command
 * */
int systemV2(char *command)
{
    int status;
    char **commands = str_split(command, ' ');
    int b_in = builtin_parser(commands);
    if (!b_in)
        return b_in;
    // printf("Builint : %d\n", b_in);
    search_replace_var(commands);
    pid_t pid = fork();
    cmd_pid = pid;
    if (pid == ERR)
    {
        perror("Fork");
        return ERR;
    }

    if (!pid)
    {

        // pid_t pgid = getpid();
        // setpgid(getpid(), pgid);
        // Put to sleep exec
        // tcsetpgrp(STDIN_FILENO, pgid);
        // printf("child pid : %d\n", getpid());
        // execl("/bin/sh", "sh", "-c", command, (const char *)0);
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
        free(commands);

        // perror("execvp failed");
        exit(FAILED_EXEC);
    }

    // pid_t pgid = pid;
    // setpgid(pid, pgid);
    // Put to sleep mysh
    // tcsetpgrp(STDIN_FILENO, pgid);

    if (wait(&status) == ERR)
    {
        perror("wait");
        return ERR;
    }
    // if (waitpid(pid, &status, WNOHANG) == ERR)
    // {
    //     perror("wait");
    //     return ERR;
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
            // printf("Lance exec sur: (%s)\n", *(commands + i));
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
            printf("var: %s\n", *(commands + i));
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

int main(int argv, char *argc[])
{
    if (argv > 1)
    {
        printf("Aucun argument n'est requis pour '%s'\n", argc[1]);
        exit(1);
    }

    mysh();

    return 0;
}
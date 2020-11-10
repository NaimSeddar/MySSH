#include "mysh.h"
#include "utils.h"

/**
 * Réécriture de la fonction system() vu en cours.
 * Version moins lourde, basé sur la fonction execvp. 
 * @param command
 * */
int systemV2(char **command)
{
    int status;
    pid_t pid = fork();

    // for (int i = 0; *(command + i); i++)
    // {
    //     printf("<%s> ", *(command + i));
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
        execvp(*command, command);
        while (*(command++))
        {
            free(*command);
        }
        free(command);
        exit(FAILED_EXEC);
    }

    if (wait(&status) == ERR)
    {
        return ERR;
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

    if (write(STDOUT_FILENO, currpath, strlen(currpath)) == ERR)
        perror("Write"), exit(1);
}

/**
 * Fonction vidant le buffer
 * @param buffer
 * */
void clearBuffer(char *buffer)
{
    while (*buffer != '\0')
    {
        *(buffer++) = '\0';
    }
}

int main(int argv, char *argc[])
{
    if (argv > 1)
    {
        printf("Aucun argument n'est requis pour '%s'\n", argc[1]);
        exit(1);
    }

    char *buffer;
    char **commands;

    for (;;)
    {
        printprompt();
        // printf("%s$ ", currpath);

        buffer = malloc(BUFFER_SIZE * sizeof(char));

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
            commands = str_split(buffer, ';');
            // if "cd" --> chdir(arg)
            // buffer = strtok(buffer, ';');
            for (int i = 0; *(commands + i); i++)
            {
                // printf("<%s>\n", str_split(*(commands + i), ' ')[0]);
                printf("status: %d\n", systemV2(str_split(*(commands + i), ' ')));
                free(*(commands + i));
            }
            free(commands);
        }
        free(buffer);
        // clearBuffer(buffer);
    }

    return 0;
}
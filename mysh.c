#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define ERR -1
#define FAILED_EXEC 127

/**
 * Réécriture de la fonction system() vu en cours.
 * Version moins lourde, basé sur la fonction execl. 
 * @param command
 * */
int systemV2(char *command)
{
    int status;
    pid_t pid = fork();

    if (pid == ERR)
    {
        perror("Fork");
        return ERR;
    }

    if (!pid)
    {
        execl("/bin/sh", "sh", "-c", command, (const char *)0);
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

        // printf("Aucun argument n'est requis pour '%s'\n", argc[1]);
        // exit(1);
    }

    char *buffer = malloc(BUFFER_SIZE * sizeof(char));

    for (;;)
    {
        if (write(STDOUT_FILENO, "\033[0;35m > \033[0m", 15) == ERR)
            perror("Write"), exit(1);

        if (read(STDIN_FILENO, buffer, 1024) == ERR)
            perror("Read"), exit(2);

        if (strcmp(buffer, "exit\n") == 0)
        {
            // printf("strcmp: %d\n", strcmp(buffer, "exit"));
            free(buffer);
            break;
        }
        else
        {
            // if "cd" --> chdir(arg)
            systemV2(buffer);
        }
        clearBuffer(buffer);
    }

    return 0;
}
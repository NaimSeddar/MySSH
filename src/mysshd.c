/**
 * Auteur:                Seddar Naïm
 * Création:              01/12/2020 18:17:34
 * Dernière modification: 30/12/2020 12:16:38
 * Master 1 Informatique
 */
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include "../includes/myssh-server.h"
#include "../includes/error.h"

int sockets[SOMAXCONN];

void signal_callback_handler(int signum)
{
    waitpid(-1, NULL, WNOHANG);
}

int main()
{
    signal(SIGCHLD, signal_callback_handler);
    int i = 0;
    int status;

    Server daemon = server_create_tcp();
    daemon->server_bind(daemon, 1344);

    if (listen(daemon->socket, SOMAXCONN) == ERR)
    {
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        pid_t pid;
        int t = i;
        sockets[i] = accept(daemon->socket, (struct sockaddr *)&daemon->clientAddr, &daemon->len);

        if ((pid = fork()) == 0)
        {
            close(daemon->socket);
            char buff[5];
            sprintf(buff, "%d", sockets[t]);
            execl("./myssh-server", "myssh-server", buff, (void *)0);
            exit(EXIT_SUCCESS);
        }

        if (waitpid(-1, &status, WNOHANG) == ERR)
        {
            perror("wait");
        }

        i++;
    }

    server_destroy(daemon);

    return 0;
}
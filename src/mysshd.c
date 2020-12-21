/**
 * Auteur:                Seddar Naïm
 * Création:              01/12/2020 18:17:34
 * Dernière modification: 21/12/2020 17:34:04
 * Master 1 Informatique
 */
#include <signal.h>
#include <errno.h>
#include "../includes/myssh-server.h"
#define MAX 500
#define str(x) #x

int sockets[SOMAXCONN];

void signal_callback_handler(int signum)
{
    printf("SIGNAL RECEPTIONNE (child: ) \n");
    waitpid(-1, NULL, WNOHANG);
    wait(NULL);
}

int main()
{
    signal(SIGCHLD, signal_callback_handler);
    int i = 0;
    int status;

    Server daemon = server_create_tcp();
    daemon->server_bind(daemon, 1344);

    if (listen(daemon->socket, SOMAXCONN) == -1)
    {
        exit(EXIT_FAILURE);
    }

    printf("Daemon à l'écoute...\n");

    for (;;)
    {
        pid_t pid;
        int t = i;
        sockets[i] = accept(daemon->socket, (struct sockaddr *)&daemon->clientAddr, &daemon->len);
        printf("Accepted client %d\n", sockets[i]);

        if ((pid = fork()) == 0)
        {
            close(daemon->socket);
            char buff[5];
            sprintf(buff, "%d", sockets[t]);
            execl("./myssh-server", "myssh-server", buff, (void *)0);
            exit(EXIT_SUCCESS);
        }

        if (waitpid(-1, &status, WNOHANG) == -1)
        {
            perror("wait");
        }

        i++;
    }

    server_destroy(daemon);

    return 0;
}
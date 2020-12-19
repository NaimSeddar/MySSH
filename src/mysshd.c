/**
 * Auteur:                Seddar Naïm
 * Création:              01/12/2020 18:17:34
 * Dernière modification: 19/12/2020 17:30:54
 * Master 1 Informatique
 */

#include <stdio.h>
#include <memory.h>
#include <pthread.h>
#include <stdlib.h>
#include <wait.h>
#include "../includes/myssh-server.h"
#define MAX 500
#define str(x) #x

int sockets[SOMAXCONN];

int main()
{

    int i = 0;
    int status;

    Server server1 = server_create_tcp();
    server1->server_bind(server1, 1344);

    if (listen(server1->socket, SOMAXCONN) == -1)
    {
        exit(EXIT_FAILURE);
    }

    printf("Listen OK\n");

    for (;;)
    {
        int t = i;
        sockets[i] = accept(server1->socket, (struct sockaddr *)&server1->clientAddr, &server1->len);
        printf("Accepted client %d\n", sockets[i]);
        pid_t pid;
        // pthread_t thread1;
        // pthread_create(&thread1, NULL, server_launch, &sockets[t]);
        // pthread_detach(thread1);
        if ((pid = fork()) == 0)
        {
            char buff[5];
            sprintf(buff, "%d", sockets[t]);
            execl("./myssh-server", "myssh-server", buff, (void *)0);
        }

        if (waitpid(pid, &status, WNOHANG) == -1)
        {
            perror("wait");
        }
        i++;
    }

    server_destroy(server1);

    return 0;
}
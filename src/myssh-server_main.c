/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 19/12/2020 15:29:56
 * Master 1 Informatique
 */

#include <stdio.h>
#include <memory.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include "../includes/myssh-server.h"
#include "../includes/data_struct.h"
#define MAX 500

// int sockets[SOMAXCONN];

void signal_callback_handler(int signum)
{
    printf("Caught signal SIGPIPE %d\n", signum);
}

void *ping(int s)
{
    // signal(SIGPIPE, signal_callback_handler);
    // int index = *((int *)p_data) - 1;
    // int s = sockets[index];
    printf("(Server) Client : %d\n", s);
    int n;
    char *msg = "bien bg";
    // char buf[MAX];
    struct auth_data p;
    for (;;)
    {
        // memset(buf, 0, sizeof(char) * MAX);
        printf("1\n");
        // memset(&p, 0, sizeof(struct auth_data));
        printf("2\n");
        if ((n = recv(s, &p, sizeof(struct auth_data), 0)) == -1)
        {
            perror("recv");
            break;
        }
        printf("3\n");
        // buf[n - 1] = '\0';
        p.specific_method_fields[strlen(p.specific_method_fields) - 1] = '\0';
        printf("4\n");
        printf("(Server) J'ai reçu : (%s) de la part de %d\n", p.specific_method_fields, s);

        printf("%d\n", p.ssh_request);
        server_send_tcp2(s, msg);
        // free(p.specific_method_fields);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    printf("(Server) argv 1 : %s\n", argv[1]);
    ping(atoi(argv[1]));

    return 0;
}
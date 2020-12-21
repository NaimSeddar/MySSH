/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 21/12/2020 16:59:15
 * Master 1 Informatique
 */

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
    struct auth_data p;

    for (;;)
    {
        if ((n = recv(s, &p, sizeof(struct auth_data), 0)) == -1)
        {
            perror("recv");
            exit(EXIT_FAILURE);
        }

        p.specific_method_fields[strlen(p.specific_method_fields) - 1] = '\0';

        printf("(Server) J'ai reçu : (%s) de la part de %d\n", p.specific_method_fields, s);

        if (strncmp(p.specific_method_fields, "exit", 4) == 0)
            break;

        printf("%d\n", p.ssh_request);

        server_send_tcp2(s, msg);
    }
    exit(EXIT_SUCCESS);
    // printf("JE SORS !");
    // fflush(stdout);
    // kill(getpid(), SIGTERM);
}

int main(int argc, char *argv[])
{
    signal(SIGPIPE, signal_callback_handler);
    printf("(Server) argv 1 : %s\n", argv[1]);
    ping(atoi(argv[1]));

    return 0;
}
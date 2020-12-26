/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 26/12/2020 22:18:08
 * Master 1 Informatique
 */

#include "../includes/myssh-server.h"
#include "../includes/data_struct.h"
#define MAX 1024

void signal_callback_handler(int signum)
{
    printf("Caught signal SIGPIPE %d\n", signum);
}

int main(int argc, char *argv[])
{
    int s = atoi(argv[1]);
    signal(SIGPIPE, signal_callback_handler);

    Server server = server_create_tcp();
    server->socket = s;

    // int yes = 1;
    // int res = setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (char *)&yes, sizeof(int));

    authenticate_client(server);

    getChannel(server);

    server_destroy(server);

    return 0;
}
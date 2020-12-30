/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 30/12/2020 12:13:00
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

    authenticate_client(server);

    getChannel(server);

    server_destroy(server);

    return 0;
}
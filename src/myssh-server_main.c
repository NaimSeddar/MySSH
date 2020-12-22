/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 22/12/2020 16:06:40
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
    signal(SIGPIPE, signal_callback_handler);

    Server server = server_create_tcp();

    authenticate_client(atoi(argv[1]));

    server_destroy(server);

    return 0;
}
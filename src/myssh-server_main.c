/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 23/12/2020 22:25:52
 * Master 1 Informatique
 */

#include "../includes/myssh-server.h"
#include "../includes/data_struct.h"
#define MAX 1024

void signal_callback_handler(int signum)
{
    printf("Caught signal SIGPIPE %d\n", signum);
}

void getChannel(struct server *this)
{
    struct channel_data ch_d;
    int save;

    this->server_receive(this, &ch_d, sizeof(struct channel_data));

    save = dup(fileno(stdout));
    dup2(this->socket, fileno(stdout));

    printf("(%s) - (%s)\n", ch_d.service_name, ch_d.command);

    printf("\n");
    putc('\0', stdout);

    fflush(stdout);
    dup2(save, fileno(stdout));
    close(save);

    clearerr(stdout);

    // this->server_send(this, '\0', 1);
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
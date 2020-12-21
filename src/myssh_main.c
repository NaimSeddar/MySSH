/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 21/12/2020 16:58:19
 * Master 1 Informatique
 */

#include "../includes/myssh.h"
#include "../includes/data_struct.h"

#define SIZE 500
#define DEBUG 1

#ifdef DEBUG
#define debug(m, ...) prinf("DEBUG: %s\n"##__VA_ARGS__)
#else
#define debug(m, ...)
#endif

static void get_msg(char *msg)
{
    fprintf(stdout, "Tapez votre message: \n");
    fgets(msg, SIZE, stdin);
    msg[strlen(msg)] = '\0';
}

int main(void)
{
    Client clt = client_create_tcp("127.0.0.1", 1344);
    struct auth_data p;
    char buffer_send[SIZE];
    char buffer_recv[SIZE];
    memset(buffer_send, 0, sizeof(char) * SIZE);
    memset(buffer_recv, 0, sizeof(char) * SIZE);

    if (connect(clt->socket, (const struct sockaddr *)&clt->clientAddr, clt->len) == -1)
    {
        perror("connect");
        exit(1);
    }

    for (;;)
    {
        get_msg(buffer_send);

        p.ssh_request = SSH_MSG_USERAUTH_REQUEST;
        memcpy(p.user_name, "Seddar", sizeof("Seddar"));
        memcpy(p.service_name, "ssh", sizeof("ssh"));
        memcpy(p.method_name, "password", sizeof("password"));
        memcpy(p.specific_method_fields, buffer_send, sizeof(buffer_send));

        printf("send : (%s) %ld\n", p.specific_method_fields, send(clt->socket, &p, sizeof(struct auth_data), MSG_NOSIGNAL));

        if (strncmp(buffer_send, "exit", 4) == 0)
        {
            break;
        }

        ssize_t n = clt->client_receive(clt, buffer_recv, SIZE);
        buffer_recv[n] = '\0';
        printf("%s\n", buffer_recv);
    }

    client_destroy(clt);
    exit(0);
}

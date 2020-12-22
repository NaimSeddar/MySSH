/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 22/12/2020 21:27:03
 * Master 1 Informatique
 */

#include "../includes/myssh.h"
#include "../includes/data_struct.h"

void oneshotcmd(Client this, char *command)
{
    printf("Executons -> (%s)\n", command);

    client_destroy(this);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./myssh <username>@<dotted address>\n");
        exit(EXIT_FAILURE);
    }

    char *addr = strchr(argv[1], '@');
    *(addr++) = '\0';
    char *username = argv[1];

    Client clt = client_create_tcp(addr, 1344);
    int c_arg = 0;
    int i;
    // char buffer_send[SIZE];
    // char buffer_recv[SIZE];
    // memset(buffer_send, 0, sizeof(char) * SIZE);
    // memset(buffer_recv, 0, sizeof(char) * SIZE);

    if (connect(clt->socket, (const struct sockaddr *)&clt->clientAddr, clt->len) == -1)
    {
        perror("connect");
        exit(1);
    }

    for (i = 0; i < argc; i++)
    {
        if (strncmp(*(argv + i), "-c", 2) == 0)
        {
            c_arg++;
            break;
        }
    }

    authenticate_to_server(clt, username);

    if (c_arg)
    {
        char *command = string_fusion(argv + i + 1);
        oneshotcmd(clt, command);
        free(command);
    }

    /*for (;;)
    {
        // get_msg(buffer_send);

        p.ssh_request = SSH_MSG_USERAUTH_REQUEST;
        memcpy(p.user_name, "Seddar", sizeof("Seddar"));
        memcpy(p.service_name, "ssh", sizeof("ssh"));
        memcpy(p.method_name, "password", sizeof("password"));
        memcpy(p.specific_method_fields, buffer_send, sizeof(buffer_send));

        printf("send : (%s) %ld\n", p.specific_method_fields, send(clt->socket, &p, sizeof(struct auth_data), MSG_NOSIGNAL));
        // printf("send : (%s) %ld\n", p.specific_method_fields, send(clt->socket, &p.specific_method_fields, strlen(p.specific_method_fields), MSG_NOSIGNAL));

        if (strncmp(buffer_send, "exit", 4) == 0)
        {
            break;
        }

        ssize_t n = clt->client_receive(clt, buffer_recv, SIZE);

        buffer_recv[n] = '\0';
        printf("%s\n", buffer_recv);
    }*/

    client_destroy(clt);
    exit(0);
}

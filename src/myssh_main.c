/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 30/12/2020 12:14:14
 * Master 1 Informatique
 */

#include "../includes/myssh.h"
#include "../includes/data_struct.h"
#include "../includes/error.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./myssh <username>@<dotted address>\n");
        exit(EXIT_FAILURE);
    }

    char addr[1024];
    char username[1024];
    char host[1024];
    char *buffer;
    int tmp;
    int c_arg = 0;
    int i;

    if (!(tmp = getHost(argv[1], addr, username, host)))
    {
        if (!strchr(argv[1], '@'))
        {
            printf("Usage: ./myssh <username>@<dotted address>\n");
            exit(EXIT_FAILURE);
        }
        buffer = strchr(argv[1], '@') + 1;

        memcpy(addr, buffer, strlen(buffer));

        *(--buffer) = '\0';
        memcpy(username, argv[1], strlen(argv[1]) + 1);
    }

    Client clt = client_create_tcp(addr, 1344);

    if (!tmp)
        memcpy(clt->host, argv[1], strlen(argv[1]) + 1);
    else
        memcpy(clt->host, host, strlen(host) + 1);

    if (connect(clt->socket, (const struct sockaddr *)&clt->clientAddr, clt->len) == ERR)
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
    else
    {
        command_loop(clt);
    }

    client_destroy(clt);
    exit(0);
}

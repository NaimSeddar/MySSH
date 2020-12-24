/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 24/12/2020 21:58:25
 * Master 1 Informatique
 */

#include "../includes/myssh.h"
#include "../includes/data_struct.h"

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

    printf("(%s) at (%s)\n", username, addr);

    Client clt = client_create_tcp(addr, 1344);
    int c_arg = 0;
    int i;

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
    else
    {
        command_loop(clt);
    }

    client_destroy(clt);
    exit(0);
}

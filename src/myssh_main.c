/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 24/12/2020 16:35:33
 * Master 1 Informatique
 */

#include "../includes/myssh.h"
#include "../includes/data_struct.h"
#include "../includes/colors.h"

void print_pcode(int pcode)
{
    printf("%sprocessus distant terminé avec le code %s[%d]%s\n", GREEN_C, YELLOW_C, pcode, RESET_C);
}

void print_socket(Client this)
{
    char buffer[SIZE];
    int n;

    buffer[SIZE - 1] = '\0';

    while ((n = this->client_receive(this, &buffer, 2)) != 0)
    {
        printf("%s", buffer);
        if (buffer[n - 1] == '\0')
            break;
    }
}

void oneshotcmd(Client this, char *command)
{
    // printf("Executons -> (%s)\n", command);

    struct channel_data ch_d;
    struct channel_data_response ch_r;
    // char buffer[SIZE];
    // int n;
    ch_d.ssh_request = SSH_MSG_CHANNEL_REQUEST;
    memcpy(ch_d.service_name, "exec\0", 6);
    memcpy(ch_d.command, command, strlen(command) + 1);

    this->client_send(this, &ch_d, sizeof(struct channel_data));

    /*buffer[2] = '\0';
    while ((n = this->client_receive(this, &buffer, 2)) != 0)
    {
        printf("%s", buffer);
        if (buffer[n - 1] == '\0')
            break;
    }*/

    print_socket(this);

    this->client_receive(this, &ch_r, sizeof(struct channel_data_response));

    print_pcode(ch_r.pcode);
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

    printf("(%s) at (%s)\n", username, addr);

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

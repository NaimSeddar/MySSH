/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 22/12/2020 14:48:44
 * Master 1 Informatique
 */

#include <termios.h>
#include "../includes/myssh.h"
#include "../includes/data_struct.h"

#define SIZE 1024

void getstdin(char *buffer, const char *prompt)
{
    if (prompt != NULL)
        printf("%s", prompt);

    fflush(stdout);

    fgets(buffer, SIZE, stdin);

    buffer[strlen(buffer) - 1] = '\0';
}

void getpassword(char *buffer)
{
    static struct termios old_terminal;
    static struct termios new_terminal;

    tcgetattr(STDIN_FILENO, &old_terminal);

    new_terminal = old_terminal;
    new_terminal.c_lflag = new_terminal.c_lflag & ~(ECHO);

    printf("Password> ");

    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);

    getstdin(buffer, NULL);

    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);

    putchar('\n');
}

void authenticate_to_server(Client this, char *username)
{
    char buffer[SIZE];
    struct auth_data p;
    struct auth_data_response r;
    ssize_t n;

    getpassword(buffer);

    p.ssh_request = SSH_MSG_USERAUTH_REQUEST;
    memcpy(p.user_name, username, strlen(username));
    memcpy(p.service_name, "ssh", 4);
    memcpy(p.method_name, "password", 9);
    memcpy(p.specific_method_fields, buffer, sizeof(buffer));

    this->client_send(this, &p, sizeof(struct auth_data));

    if ((n = this->client_receive(this, &r, sizeof(struct auth_data_response))) == -1)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    if (r.ssh_request == SSH_MSG_USERAUTH_FAILURE)
    {
        printf("%s\n", r.message);
    }
    else
    {
        printf("C'est bien ça !\n");
    }

    fflush(stdout);
    client_destroy(this);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    char *addr = strchr(argv[1], '@');
    *(addr++) = '\0';
    char *username = argv[1];
    // printf("User: (%s) at Address: (%s)\n", username, addr);

    Client clt = client_create_tcp(addr, 1344);
    // struct auth_data p;
    char buffer_send[SIZE];
    char buffer_recv[SIZE];
    memset(buffer_send, 0, sizeof(char) * SIZE);
    memset(buffer_recv, 0, sizeof(char) * SIZE);

    if (connect(clt->socket, (const struct sockaddr *)&clt->clientAddr, clt->len) == -1)
    {
        perror("connect");
        exit(1);
    }

    authenticate_to_server(clt, username);

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

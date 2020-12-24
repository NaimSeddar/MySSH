/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 24/12/2020 22:30:58
 * Master 1 Informatique
 */

#include "../includes/myssh.h"
#include "../includes/error.h"
#include "../includes/data_struct.h"
#include "../includes/colors.h"

#define neterr_client(clt, n) client_destroy(clt), syserror(n);

ssize_t client_receive_tcp(struct client *this, void *data, size_t size_data)
{
    return recv(this->socket, data, size_data, MSG_NOSIGNAL);
}

void client_send_tcp(struct client *this, void *data, size_t size_data)
{
    if (send(this->socket, data, size_data, MSG_NOSIGNAL) == ERR)
        neterr_client(this, SEND_ERR);
}

Client client_create_tcp(char *addr, int port)
{
    Client clt = malloc(sizeof(struct client));
    int sfd;

    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == ERR)
    {
        neterr_client(clt, SOCKET_ERR);
    }

    memset(&clt->clientAddr, 0, sizeof(struct sockaddr_in));

    clt->socket = sfd;
    clt->clientAddr.sin_family = AF_INET;
    clt->clientAddr.sin_port = htons((uint16_t)port);
    clt->client_receive = &client_receive_tcp;
    clt->client_send = &client_send_tcp;
    clt->len = sizeof(struct sockaddr_in);

    if (!inet_aton(addr, &clt->clientAddr.sin_addr))
    {
        neterr_client(clt, SOCKET_ERR);
    }

    return clt;
}

void client_destroy(Client this)
{
    close(this->socket);
    free(this);
}

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
    memcpy(p.user_name, username, strlen(username) + 1);
    memcpy(p.service_name, "ssh", 4);
    memcpy(p.method_name, "password", 9);
    memcpy(p.specific_method_fields, buffer, sizeof(buffer));

    this->client_send(this, &p, sizeof(struct auth_data));

    if ((n = this->client_receive(this, &r, sizeof(struct auth_data_response))) == -1)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    if (r.ssh_answer == SSH_MSG_USERAUTH_FAILURE)
    {
        printf("%s\n", r.message);
        client_destroy(this);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("C'est bien ça !\n");
    }

    fflush(stdout);
}

char *string_fusion(char **strings)
{
    char *buffer;
    int total_len = 1;

    for (int i = 0; *(strings + i) != NULL; i++)
        total_len += strlen(*(strings + i)) + 1;

    buffer = (char *)malloc(sizeof(char) * total_len);

    strcpy(buffer, *strings++);

    while (*strings != NULL)
    {
        strcat(buffer, " ");
        strcat(buffer, *(strings++));
    }

    return buffer;
}

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

    struct channel_data ch_d;
    struct channel_data_response ch_r;

    ch_d.ssh_request = SSH_MSG_CHANNEL_REQUEST;

    memcpy(ch_d.service_name, "exec\0", 6);
    memcpy(ch_d.command, command, strlen(command) + 1);

    this->client_send(this, &ch_d, sizeof(struct channel_data));

    print_socket(this);

    this->client_receive(this, &ch_r, sizeof(struct channel_data_response));

    print_pcode(ch_r.pcode);
}

void command_loop(Client this)
{
    struct channel_data ch_d;

    ch_d.ssh_request = SSH_MSG_CHANNEL_REQUEST;
    memcpy(ch_d.service_name, "shell", 6);

    printf("%sJe vais envoyer dans command_loop\n", GREEN_C);
    this->client_send(this, &ch_d, SIZEOF_CH_D);
    printf("%sJ'ai envoyé dans command_loop\n", RED_C);

    printf("%sJe vais recevoir dans command_loop\n", GREEN_C);
    this->client_receive(this, &ch_d, SIZEOF_CH_D);
    printf("%sJ'ai reçu dans command_loop\n", RED_C);

    /*if (ch_d.ssh_request == SSH_MSG_CHANNEL_FAILURE)
    {
        printf("%sConnexion refusé\n", RED_C);
        client_destroy(this);
        exit(EXIT_FAILURE);
    }*/

    printf("%s%s>\n", YELLOW_C, ch_d.command);
}
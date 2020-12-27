/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 27/12/2020 12:42:14
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

    if ((n = this->client_receive(this, &r, SIZEOF_AUTH_R)) == -1)
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

    // sleep(1);

    while ((n = this->client_receive(this, &buffer, 2)) != 0)
    {
        printf("%s", buffer);
        if (buffer[n - 1] == '\0')
        {
            break;
        }
    }

    // clearerr(stdout);
    // fflush(stdout);
}

void oneshotcmd(Client this, char *command)
{

    struct channel_data ch_d;
    struct channel_data_response ch_r;

    ch_d.ssh_request = SSH_MSG_CHANNEL_REQUEST;

    memcpy(ch_d.service_name, "exec\0", 6);
    memcpy(ch_d.command, command, strlen(command) + 1);

    this->client_send(this, &ch_d, SIZEOF_CH_D);

    print_socket(this);

    // sleep(1);

    this->client_receive(this, &ch_r, SIZEOF_CH_R);

    // sleep(1);

    print_pcode(ch_r.pcode);
    // printf("%d %d %s", ch_r.ssh_answer, ch_r.pcode, ch_r.comment);
}

void command_loop(Client this)
{
    struct channel_data ch_d;
    struct channel_data_response ch_r;
    char buffer[4095];
    int ack = 1;

    ch_d.ssh_request = SSH_MSG_CHANNEL_REQUEST;

    memcpy(ch_d.service_name, "shell\0", 7);

    this->client_send(this, &ch_d, SIZEOF_CH_D);

    this->client_receive(this, &ch_r, SIZEOF_CH_R);
    printf("%s<<%d> <%d> <%s>>%s\n", RED_C, ch_r.ssh_answer, ch_r.pcode, ch_r.comment, RESET_C);

    for (int i = 0; i < 1; i++)
    {
        printf("%s", ch_r.comment);
        getstdin(buffer, "> ");
        memcpy(ch_d.command, buffer, strlen(buffer) + 1);

        printf("%sJ'envoi une commande au serveur\n", RED_C);
        this->client_send(this, &ch_d, SIZEOF_CH_D);
        printf("%sCommande (%s) envoyé%s\n", GREEN_C, ch_d.command, RESET_C);

        /*if (strncmp(ch_d.command, "exit", 4) == 0)
        {
            this->client_receive(this, &ch_r, SIZEOF_CH_R);
            client_destroy(this);
            exit(EXIT_SUCCESS);
        }*/

        printf("%sDébut lecture socket\n", RED_C);
        print_socket(this);
        printf("%sSocket lu :ok_hand:\n", GREEN_C);

        this->client_send(this, &ack, sizeof(int));

        // sleep(1);
        // fflush(stdout);
        // printf("%s<ACK>%s\n", YELLOW_C, RESET_C);
        printf("%sJ'attends le retour du serveur\n", RED_C);
        this->client_receive(this, &ch_r, SIZEOF_CH_R);
        printf("%sLe serveur me dit %d\n", GREEN_C, ch_r.ssh_answer);

        // print_pcode(ch_r.pcode);
        printf("%s<<%d> <%d> <%s>>%s\n", YELLOW_C, ch_r.ssh_answer, ch_r.pcode, ch_r.comment, RESET_C);
        // fflush(stdout);
    }
}
/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 22/12/2020 21:29:12
 * Master 1 Informatique
 */

#include "../includes/myssh-server.h"
#include "../includes/error.h"
#include "../includes/data_struct.h"

#define neterr_server(srv, n) server_destroy(srv), syserror(n);
#define MAX 500

/*
static ssize_t server_receive_tcp(struct server *this, char *buf, size_t size)
{
    return recv(this->acceptedSocket, buf, size, 0);
}

static void server_send_tcp(struct server *this, char *msg)
{
    if (send(this->acceptedSocket, msg, strlen(msg), 0) == ERR)
    {
        neterr_server(this, SEND_ERR);
    }
}*/

ssize_t server_receive_tcp(int socket, void *data, size_t size)
{
    return recv(socket, data, size, 0);
}

void server_send_tcp(int socket, void *data, size_t data_size)
{
    if (send(socket, data, data_size, MSG_NOSIGNAL) == -1)
    {
        syserror(SEND_ERR);
        exit(EXIT_SUCCESS);
    }
}

static void server_bind(struct server *this, int port)
{
    this->servAddr.sin_family = AF_INET;
    this->servAddr.sin_addr.s_addr = INADDR_ANY;
    this->servAddr.sin_port = htons((uint16_t)port);
    if (bind(this->socket, (struct sockaddr *)&this->servAddr, sizeof(this->servAddr)) < 0)
    {
        neterr_server(this, BINDING_ERR);
    }
}

Server server_create_tcp()
{
    Server srv = malloc(sizeof(struct server));

    int sfd;
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == ERR)
    {
        free(srv);
        syserror(SOCKET_ERR);
    }

    srv->socket = sfd;
    srv->server_bind = &server_bind;
    // srv->server_receive = &server_receive_tcp;
    // srv->server_send = &server_send_tcp;
    memset(&srv->servAddr, 0, sizeof(struct sockaddr_in));
    memset(&srv->clientAddr, 0, sizeof(struct sockaddr_in));
    srv->len = sizeof(struct sockaddr_in);
    return srv;
}

void server_destroy(Server this)
{
    close(this->socket);
    free(this);
}

struct auth_data_response check_credentials(char *username, char *clear_password)
{
    struct auth_data_response res;
    struct passwd *p;
    struct spwd *sp;
    char *error_msg = "Invalid username/password...";
    size_t error_msg_len = strlen(error_msg);

    if (setuid(0) == -1)
    {
        perror("Permissions issues (setuid)");
        exit(EXIT_FAILURE);
    }

    if ((p = getpwnam(username)) == NULL)
    {
        res.ssh_request = SSH_MSG_USERAUTH_FAILURE;
        memcpy(res.message, error_msg, error_msg_len + 1);
        res.message[error_msg_len] = '\0';

        return res;
    }

    if (strncmp(p->pw_passwd, "x", 1) == 0)
    {
        if ((sp = getspnam(username)) == NULL)
        {
            perror("Permissions issues (shadow password)");
            exit(EXIT_FAILURE);
        }

        p->pw_passwd = sp->sp_pwdp;
    }

    if (strcmp(crypt(clear_password, p->pw_passwd), p->pw_passwd) == 0)
    {
        res.ssh_request = SSH_MSG_USERAUTH_SUCCESS;
        return res;
    }
    else
    {
        res.ssh_request = SSH_MSG_USERAUTH_FAILURE;
        memcpy(res.message, error_msg, error_msg_len + 1);
        res.message[error_msg_len] = '\0';

        return res;
    }
}

void authenticate_client(int s)
{
    int n;
    struct auth_data p;
    struct auth_data_response r;

    if ((n = recv(s, &p, sizeof(struct auth_data), 0)) == -1)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    r = check_credentials(p.user_name, p.specific_method_fields);

    server_send_tcp(s, &r, sizeof(struct auth_data_response));
}
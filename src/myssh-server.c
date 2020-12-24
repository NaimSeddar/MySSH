/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 24/12/2020 14:42:18
 * Master 1 Informatique
 */

#include "../includes/myssh-server.h"
#include "../includes/error.h"
#include "../includes/data_struct.h"
#include "../includes/mysh.h"

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

ssize_t server_receive_tcp(struct server *this, void *data, size_t size)
{
    return recv(this->socket, data, size, 0);
}

void server_send_tcp(struct server *this, void *data, size_t data_size)
{
    if (send(this->socket, data, data_size, MSG_NOSIGNAL) == -1)
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
    srv->server_receive = &server_receive_tcp;
    srv->server_send = &server_send_tcp;
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

void update_user(struct passwd *p)
{
    setgid(p->pw_gid);
    setuid(p->pw_uid);
    seteuid(p->pw_uid);

    setenv("HOME", p->pw_dir, 0);
    setenv("USER", p->pw_name, 1);
    setenv("SHELL", p->pw_shell, 1);
    setenv("LOGNAME", p->pw_name, 1);
}

struct auth_data_response check_credentials(char *username, char *clear_password)
{
    struct auth_data_response res;
    struct passwd *p;
    struct spwd *sp;
    char *error_msg = "Invalid username/password...";
    char *error_msg2 = "Unknown user";
    size_t error_msg_len = strlen(error_msg);
    size_t error_msg_len2 = strlen(error_msg2);

    if (setuid(0) == -1)
    {
        perror("Permissions issues (setuid)");
        exit(EXIT_FAILURE);
    }

    if ((p = getpwnam(username)) == NULL)
    {
        res.ssh_answer = SSH_MSG_USERAUTH_FAILURE;
        memcpy(res.message, error_msg2, error_msg_len2 + 1);
        res.message[error_msg_len2] = '\0';

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
        res.ssh_answer = SSH_MSG_USERAUTH_SUCCESS;
        update_user(p);
    }
    else
    {
        res.ssh_answer = SSH_MSG_USERAUTH_FAILURE;
        memcpy(res.message, error_msg, error_msg_len + 1);
        res.message[error_msg_len] = '\0';
    }

    return res;
}

void authenticate_client(struct server *this)
{
    int n;
    struct auth_data p;
    struct auth_data_response r;

    // if ((n = recv(s, &p, sizeof(struct auth_data), 0)) == -1)
    if ((n = this->server_receive(this, &p, sizeof(struct auth_data))) == -1)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    r = check_credentials(p.user_name, p.specific_method_fields);

    // server_send_tcp(s, &r, sizeof(struct auth_data_response));
    this->server_send(this, &r, sizeof(struct auth_data_response));

    if (r.ssh_answer == SSH_MSG_USERAUTH_FAILURE)
    {
        server_destroy(this);
        exit(EXIT_FAILURE);
    }
}

void getChannel(struct server *this)
{
    struct channel_data ch_d;
    int save;

    this->server_receive(this, &ch_d, sizeof(struct channel_data));

    save = dup(fileno(stdout));
    dup2(this->socket, fileno(stdout));

    // printf("(%s) - (%s)\n", ch_d.service_name, ch_d.command);

    parser(ch_d.command);

    printf("%c", '\0');

    fflush(stdout);
    dup2(save, fileno(stdout));
    close(save);

    clearerr(stdout);

    // this->server_send(this, '\0', 1);
}
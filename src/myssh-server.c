/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 21/12/2020 13:54:33
 * Master 1 Informatique
 */

#include "../includes/myssh-server.h"
#include "../includes/error.h"

#define neterr_server(srv, n) server_destroy(srv), syserror(n);
#define MAX 500

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
}

ssize_t server_receive_tcp2(int socket, char *buf, size_t size)
{
    return recv(socket, buf, size, 0);
}

void server_send_tcp2(int socket, char *msg)
{
    if (send(socket, msg, strlen(msg), MSG_NOSIGNAL) == -1)
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

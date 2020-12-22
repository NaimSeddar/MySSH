/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 22/12/2020 13:33:33
 * Master 1 Informatique
 */

#include "../includes/myssh.h"
#include "../includes/error.h"

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
        neterr_client(clt, SOCKET_ERR)
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
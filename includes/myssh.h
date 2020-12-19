/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:47
 * Dernière modification: 19/12/2020 12:16:35
 * Master 1 Informatique
 */

#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

/* header ============================================================== */
#include <zconf.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* macros ============================================================== */

/* constants =========================================================== */

/* structures ========================================================== */
struct client
{
    int socket;
    struct sockaddr_in clientAddr;
    socklen_t len;

    ssize_t (*client_receive)(struct client *this, char *buf, size_t size);
    void (*client_send)(struct client *this, char *msg);
};

/* types =============================================================== */
typedef struct client *Client;

/* functions =========================================== */

Client client_create_tcp(char *addr, int port);
void client_destroy(Client this);

#endif //UDP_CLIENT_H

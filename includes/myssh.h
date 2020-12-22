/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:47
 * Dernière modification: 22/12/2020 13:31:02
 * Master 1 Informatique
 */

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <zconf.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct client
{
    int socket;
    struct sockaddr_in clientAddr;
    socklen_t len;

    ssize_t (*client_receive)(struct client *this, void *data, size_t size);
    void (*client_send)(struct client *this, void *data, size_t size);
};

typedef struct client *Client;

Client client_create_tcp(char *addr, int port);
void client_destroy(Client this);

#endif

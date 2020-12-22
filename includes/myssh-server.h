/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 22/12/2020 12:20:51
 * Master 1 Informatique
 */

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <zconf.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <wait.h>
#include <signal.h>

struct server
{
    int socket;
    int acceptedSocket;
    struct sockaddr_in servAddr;
    struct sockaddr_in clientAddr;
    socklen_t len;

    // ssize_t (*server_receive)(struct server *this, char *buf, size_t size);
    // void (*server_send)(struct server *this, char *msg);
    void (*server_bind)(struct server *this, int port);
};

typedef struct server *Server;

Server server_create_tcp();
void server_destroy(Server this);
void server_send_tcp(int socket, void *data, size_t data_size);
ssize_t server_receive_tcp(int socket, void *data, size_t size);
// void *ping(void *p_data);

#endif
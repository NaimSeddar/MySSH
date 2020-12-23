/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 23/12/2020 16:07:22
 * Master 1 Informatique
 */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
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
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>

struct server
{
    int socket;
    int acceptedSocket;
    struct sockaddr_in servAddr;
    struct sockaddr_in clientAddr;
    socklen_t len;

    ssize_t (*server_receive)(struct server *this, void *data, size_t size);
    void (*server_send)(struct server *this, void *data, size_t size);
    void (*server_bind)(struct server *this, int port);
};

typedef struct server *Server;

Server server_create_tcp();
void server_destroy(Server this);
void server_send_tcp(struct server *this, void *data, size_t data_size);
ssize_t server_receive_tcp(struct server *this, void *data, size_t size);
struct auth_data_response check_credentials(char *username, char *clear_password);
void update_user(struct passwd *p);
void authenticate_client(struct server *this);

#endif
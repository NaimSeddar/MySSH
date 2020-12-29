/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:47
 * Dernière modification: 29/12/2020 20:53:33
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
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <termios.h>
#include <unistd.h>

#define SIZE 1024

struct client
{
    int socket;
    struct sockaddr_in clientAddr;
    socklen_t len;
    char host[SIZE];

    ssize_t (*client_receive)(struct client *this, void *data, size_t size);
    void (*client_send)(struct client *this, void *data, size_t size);
};

typedef struct client *Client;

Client client_create_tcp(char *addr, int port);
void client_destroy(Client this);
int getHost(char *hostname, char *addr, char *name, char *fullhost);
void getstdin(char *buffer, const char *prompt);
void getpassword(char *buffer);
void authenticate_to_server(Client this, char *username);
char *string_fusion(char **strings);
void print_pcode(int pcode);
void print_socket(Client this);
void oneshotcmd(Client this, char *command);
void command_loop(Client this);

#endif

/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 30/12/2020 16:25:46
 * Master 1 Informatique
 */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#ifndef SERVER_H
#define SERVER_H

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
#include <unistd.h>

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

/**
 * Create a server struct.
 * @return  A filled server struct.
 */
Server server_create_tcp();

/**
 * Destroy a given server.
 * @param  this  A server struct.
 */
void server_destroy(Server this);

/**
 * Send data into the socket assigned in the server struct.
 * @param  this       Our server.
 * @param  data       Data we want to send.
 * @param  data_size  The size of what we gonna send.
 */
void server_send_tcp(struct server *this, void *data, size_t data_size);

/**
 * Receive data from the socket assigned in the server struct.
 * @param  this       Our server.
 * @param  data       A receptacle in order to store the data.
 * @param  data_size  The size of the expected data.
 * @return            The size of the received data.
 */
ssize_t server_receive_tcp(struct server *this, void *data, size_t size);

/**
 * Bind a given server to a port.
 * @param  this  A server.
 * @param  port  A port.
 */
void server_bind(struct server *this, int port);

/**
 * Check the user credentials.
 * @param  username        The username.
 * @param  clear_password  The password entered by the user.
 * @return                 An  auth_data_response ready to be send.
 */
struct auth_data_response check_credentials(char *username, char *clear_password);

/**
 * Update the environemt with the logged user.
 * @param  p  A passwd struct with current user informations.
 */
void update_user(struct passwd *p);

/**
 * Let the client log in.
 * @param  this  Our server.
 */
void authenticate_client(struct server *this);

/**
 * Execute a given command and redirect the stdout and stderr to the socket.
 * @param  this     Our server.
 * @param  command  The command to execute.
 */
int remote_exec(Server this, char *command);

/**
 * Execute the command and send the result.
 * @param  this     Our server.
 * @param  command  The command to execute. 
 */
void oneshotexec(Server this, char *command);

/**
 * An exit handler.
 * @param  this  Our server. 
 */
void exit_process(Server this);

/**
 * An infinite loop where the client send command to process.
 * @param  this  Our server.
 */
void exec_loop(Server this);

/**
 * Find out if the client wants a one shot command, or open the shell.
 * @param  this  Our server.
 */
void getChannel(struct server *this);

#endif
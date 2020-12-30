/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:47
 * Dernière modification: 30/12/2020 16:43:26
 * Master 1 Informatique
 */

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

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

#define neterr_client(clt, n) client_destroy(clt), syserror(n);

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

/**
 * Create a client struct.
 * @param  addr  An dotted address.
 * @param  port  A port to use.
 * @return       A filled client struct.
 */
Client client_create_tcp(char *addr, int port);

/**
 * Receive data from the socket assigned in the client struct.
 * @param  this       Our client struct.
 * @param  data       A receptacle in order to store the data.
 * @param  data_size  The size of the expected data.
 * @return            The size of the received data.
 */
ssize_t client_receive_tcp(struct client *this, void *data, size_t size_data);

/**
 * Send data into the socket assigned in the client struct.
 * @param  this       Our client struct.
 * @param  data       Data we want to send.
 * @param  data_size  The size of what we gonna send.
 */
void client_send_tcp(struct client *this, void *data, size_t size_data);

/**
 * Destroy a given client.
 * @param  this  A client struct.
 */
void client_destroy(Client this);

/**
 * Get the address and name of a hostname in $HOME/.myssh/config.
 * @param  hostname          An hostname used in the config file.
 * @param  addr              A buffer used to store the address.
 * @param  name              A buffer used to store the username.
 * @param  fullhostname      A buffer used to store the entire host name. ([username]@[dotted addr])
 * @return                   1 if the hostname has been found, else 0.
 */
int getHost(char *hostname, char *addr, char *name, char *fullhost);

/**
 * Display a prompt and read stdin.
 * @param  buffer  A buffer to store inputs.
 * @param  prompt  A prompt for the user.
 */
void getstdin(char *buffer, const char *prompt);

/**
 * Read user's input without diplaying it.
 * @param  buffer  A buffer to store inputs.
 */
void getpassword(char *buffer);

/**
 * Input a password and send the credentials for a given username.
 * @param  this      Our client struct.
 * @param  username  A user we want to log as.
 */
void authenticate_to_server(Client this, char *username);

/**
 * Fuse an array of string into one spaced string.
 * @param  strings  An array of strings.
 * @return          The fused string. 
 */
char *string_fusion(char **strings);

/**
 * Return the status of the last command.
 * @param  pcode  Command status.
 */
void print_pcode(int pcode);

/**
 * Print what's in the socket stream until a '\0' is encountered.
 * @param  this  Our client struct.
 */
void print_socket(Client this);

/**
 * Print a prompt. ([username]@[address]:[path]> )
 * @param  this  Our client struct.
 * @param  path  The current path.
 */
void prompt_client(Client this, char *path);

/**
 * Ask just for one command.
 * @param  this     Our client struct.
 * @param  command  Command to execute.
 */
void oneshotcmd(Client this, char *command);

/**
 * An infinite loop where we ask th server for commands.
 * @param  this  Our client struct.
 */
void command_loop(Client this);

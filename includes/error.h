/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 30/12/2020 12:18:39
 * Master 1 Informatique
 */

#include <stdio.h>

#ifndef ERROR_H
#define ERROR_H

#define ERR -1
#define NO_ERR 0
#define SOCKET_ERR 1
#define BINDING_ERR 3
#define SEND_ERR 4
#define ACCEPT_ERR 5
#define LISTEN_ERR 6
#define READ_ERR 7
#define OPEN_FILE_ERR 8

#define syserror(n) perror(msgErr[n]), exit(n)

/**
 * Array of error messages.
 */
extern char *msgErr[];

#endif
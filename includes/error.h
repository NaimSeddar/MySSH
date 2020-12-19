/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 19/12/2020 12:17:25
 * Master 1 Informatique
 */

#ifndef UDP_ERROR_H
#define UDP_ERROR_H

#include <stdio.h>

#define ERR -1
#define NO_ERR 0
#define SIGEMPTYSET_ERR 1
#define SIGACTION_ERR 2
#define SOCKET_ERR 3
#define BINDING_ERR 4
#define SEND_ERR 5
#define ACCEPT_ERR 6
#define LISTEN_ERR 7
#define READ_ERR 8
#define THREAD_CREATE_ERR 9
#define OPEN_FILE_ERR 10

#define syserror(n) perror(msgErr[n]), exit(n)

extern char *msgErr[];

#endif //UDP_ERROR_H

/**
 * Auteur:                Seddar Naïm
 * Création:              03/12/2020 20:29:09
 * Dernière modification: 24/12/2020 19:09:28
 * Master 1 Informatique
 */

#ifndef DATA_STRUCT
#define DATA_STRUCT

#define SSH_MSG_USERAUTH_REQUEST 50
#define SSH_MSG_USERAUTH_FAILURE 51
#define SSH_MSG_USERAUTH_SUCCESS 52
#define SSH_MSG_CHANNEL_REQUEST 98
#define SSH_MSG_CHANNEL_SUCCESS 99
#define SSH_MSG_CHANNEL_FAILURE 100

#define SIZEOF_CH_D sizeof(struct channel_data)

struct auth_data
{
    int ssh_request;
    char user_name[256];
    char service_name[256];
    char method_name[128];
    char specific_method_fields[1024];
};

struct auth_data_response
{
    int ssh_answer;
    char message[128];
};

struct channel_data
{
    int ssh_request;
    char service_name[256]; // exec, shell or signal
    char command[4096];
};

struct channel_data_response
{
    int ssh_answer;
    int pcode;
};

struct loop_data
{
    int ssh_request;
    char command[4096];
};

#endif
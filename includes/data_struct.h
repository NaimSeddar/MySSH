/**
 * Auteur:                Seddar Naïm
 * Création:              03/12/2020 20:29:09
 * Dernière modification: 19/12/2020 12:13:03
 * Master 1 Informatique
 */

#define SSH_MSG_USERAUTH_REQUEST 50
#define SSH_MSG_USERAUTH_FAILURE 51
#define SSH_MSG_USERAUTH_SUCCESS 52

struct auth_data
{
    int ssh_request;
    char user_name[256];
    char service_name[256];
    char method_name[128];
    char specific_method_fields[1024];
};
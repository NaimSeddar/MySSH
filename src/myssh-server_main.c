/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 22/12/2020 14:38:48
 * Master 1 Informatique
 */

#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include "../includes/myssh-server.h"
#include "../includes/data_struct.h"
#define MAX 1024

void signal_callback_handler(int signum)
{
    printf("Caught signal SIGPIPE %d\n", signum);
}

struct auth_data_response check_credentials(char *username, char *clear_password)
{
    struct auth_data_response res;
    struct passwd *p;
    struct spwd *sp;
    char *error_msg = "Invalide username/password...";
    size_t error_msg_len = strlen(error_msg);

    if (setuid(0) == -1)
    {
        perror("Permissions issues (setuid)");
        exit(EXIT_FAILURE);
    }

    if ((p = getpwnam(username)) == NULL)
    {
        res.ssh_request = SSH_MSG_USERAUTH_FAILURE;
        memcpy(res.message, error_msg, error_msg_len + 1);
        res.message[error_msg_len] = '\0';

        return res;
    }

    if (strncmp(p->pw_passwd, "x", 1) == 0)
    {
        if ((sp = getspnam(username)) == NULL)
        {
            perror("Permissions issues (shadow password)");
            exit(EXIT_FAILURE);
        }

        p->pw_passwd = sp->sp_pwdp;
    }

    if (strcmp(crypt(clear_password, p->pw_passwd), p->pw_passwd) == 0)
    {
        res.ssh_request = SSH_MSG_USERAUTH_SUCCESS;
        return res;
    }
    else
    {
        res.ssh_request = SSH_MSG_USERAUTH_FAILURE;
        memcpy(res.message, error_msg, error_msg_len + 1);
        res.message[error_msg_len] = '\0';

        return res;
    }
}

void authenticate_client(int s)
{
    int n;
    struct auth_data p;
    struct auth_data_response r;

    if ((n = recv(s, &p, sizeof(struct auth_data), 0)) == -1)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    r = check_credentials(p.user_name, p.specific_method_fields);

    server_send_tcp(s, &r, sizeof(struct auth_data_response));
}

int main(int argc, char *argv[])
{
    signal(SIGPIPE, signal_callback_handler);

    Server server = server_create_tcp();

    authenticate_client(atoi(argv[1]));

    server_destroy(server);

    return 0;
}
/**
 * Auteur:                Seddar Naïm
 * Création:              24/11/2020 14:50:43
 * Dernière modification: 30/12/2020 11:28:09
 * Master 1 Informatique
 */

#include "../includes/myssh-server.h"
#include "../includes/error.h"
#include "../includes/data_struct.h"
#include "../includes/mysh.h"

#define neterr_server(srv, n) server_destroy(srv), syserror(n);

ssize_t server_receive_tcp(struct server *this, void *data, size_t size)
{
    ssize_t res = recv(this->socket, data, size, 0);

    if (res == -1)
    {
        server_destroy(this);
        exit(EXIT_FAILURE);
    }

    return res;
}

void server_send_tcp(struct server *this, void *data, size_t data_size)
{
    if (send(this->socket, data, data_size, MSG_NOSIGNAL) == -1)
    {
        syserror(SEND_ERR);
        exit(EXIT_SUCCESS);
    }
}

static void server_bind(struct server *this, int port)
{
    this->servAddr.sin_family = AF_INET;
    this->servAddr.sin_addr.s_addr = INADDR_ANY;
    this->servAddr.sin_port = htons((uint16_t)port);
    if (bind(this->socket, (struct sockaddr *)&this->servAddr, sizeof(this->servAddr)) < 0)
    {
        neterr_server(this, BINDING_ERR);
    }
}

Server server_create_tcp()
{
    Server srv = malloc(sizeof(struct server));

    int sfd;
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == ERR)
    {
        free(srv);
        syserror(SOCKET_ERR);
    }

    srv->socket = sfd;
    srv->server_bind = &server_bind;
    srv->server_receive = &server_receive_tcp;
    srv->server_send = &server_send_tcp;
    memset(&srv->servAddr, 0, sizeof(struct sockaddr_in));
    memset(&srv->clientAddr, 0, sizeof(struct sockaddr_in));
    srv->len = sizeof(struct sockaddr_in);
    return srv;
}

void server_destroy(Server this)
{
    close(this->socket);
    free(this);
}

void update_user(struct passwd *p)
{
    setenv("HOME", p->pw_dir, 1);
    setenv("USER", p->pw_name, 1);
    setenv("SHELL", p->pw_shell, 1);
    setenv("LOGNAME", p->pw_name, 1);

    setgid(p->pw_gid);
    setuid(p->pw_uid);
    seteuid(p->pw_uid);
}

struct auth_data_response check_credentials(char *username, char *clear_password)
{
    struct auth_data_response res;
    struct passwd *p;
    struct spwd *sp;
    char *error_msg = "Invalid username/password...";
    char *error_msg2 = "Unknown user";
    size_t error_msg_len = strlen(error_msg);
    size_t error_msg_len2 = strlen(error_msg2);

    if (setuid(0) == -1)
    {
        perror("Permissions issues (setuid)");
        exit(EXIT_FAILURE);
    }

    if ((p = getpwnam(username)) == NULL)
    {
        res.ssh_answer = SSH_MSG_USERAUTH_FAILURE;
        memcpy(res.message, error_msg2, error_msg_len2 + 1);
        res.message[error_msg_len2] = '\0';

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
        res.ssh_answer = SSH_MSG_USERAUTH_SUCCESS;
        update_user(p);
    }
    else
    {
        res.ssh_answer = SSH_MSG_USERAUTH_FAILURE;
        memcpy(res.message, error_msg, error_msg_len + 1);
        res.message[error_msg_len] = '\0';
    }

    return res;
}

void authenticate_client(struct server *this)
{
    int n;
    struct auth_data p;
    struct auth_data_response r;

    // if ((n = recv(s, &p, sizeof(struct auth_data), 0)) == -1)
    if ((n = this->server_receive(this, &p, sizeof(struct auth_data))) == -1)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    r = check_credentials(p.user_name, p.specific_method_fields);

    // server_send_tcp(s, &r, sizeof(struct auth_data_response));
    this->server_send(this, &r, sizeof(struct auth_data_response));

    if (r.ssh_answer == SSH_MSG_USERAUTH_FAILURE)
    {
        server_destroy(this);
        exit(EXIT_FAILURE);
    }

    // sleep(1);
}

int remote_exec(Server this, char *command)
{
    int save_out, save_err, n;

    save_err = dup(fileno(stderr));
    save_out = dup(fileno(stdout));

    dup2(this->socket, fileno(stderr));
    dup2(this->socket, fileno(stdout));

    n = parser(command);

    printf("%c", '\0');

    fflush(stderr);
    fflush(stdout);
    clearerr(stdout);
    clearerr(stderr);

    dup2(save_err, fileno(stderr));
    dup2(save_out, fileno(stdout));

    close(save_err);
    close(save_out);

    return n;
}

void oneshotexec(Server this, char *command)
{
    struct channel_data_response ch_r;

    int n = remote_exec(this, command);

    ch_r.ssh_answer = (n == 0 ? SSH_MSG_CHANNEL_SUCCESS : SSH_MSG_CHANNEL_FAILURE);
    ch_r.pcode = n;

    printf("Code de retour : %d (%d)\n", ch_r.pcode, ch_r.ssh_answer);

    this->server_send(this, &ch_r, SIZEOF_CH_R);
}

void exit_process(Server this)
{
    struct channel_data_response ch_r;
    ch_r.ssh_answer = SSH_MSG_CHANNEL_SUCCESS;
    this->server_send(this, &ch_r, SIZEOF_CH_R);
    server_destroy(this);
    exit(EXIT_SUCCESS);
}

void exec_loop(Server this)
{
    struct channel_data ch;
    struct channel_data_response ch_r;
    int n, ack = 0;

    ch_r.ssh_answer = SSH_MSG_CHANNEL_SUCCESS;
    getcwd(ch_r.comment, 4095);

    this->server_send(this, &ch_r, SIZEOF_CH_R);

    for (;;)
    {
        memset(ch.command, '\0', 4096);
        ch.ssh_request = 0;
        printf("%sJ'attend un commande de la part du client\n", RED_C);
        do
        {
            this->server_receive(this, &ch, SIZEOF_CH_D);
        } while (ch.ssh_request == 0);
        printf("%sLe client veut exec un : (%s)\n", GREEN_C, ch.command);

        if (ch.command[0] == '\0')
            continue;

        if (strncmp(ch.command, "exit", 4) == 0)
        {
            exit_process(this);
        }

        printf("%sJe lance l'exec\n", RED_C);
        n = remote_exec(this, ch.command);
        printf("%sExecution terminé\n", GREEN_C);

        do
        {
            this->server_receive(this, &ack, sizeof(int));
        } while (ack != 1);

        ack = 0;

        ch_r.ssh_answer = (n == 0 ? SSH_MSG_CHANNEL_SUCCESS : SSH_MSG_CHANNEL_FAILURE);
        ch_r.pcode = n;
        getcwd(ch_r.comment, 4095);

        printf("%sJ'vais envoyer le truc\n", RED_C);
        this->server_send(this, &ch_r, SIZEOF_CH_R);
        printf("%sJ'ai envoyé le truc hein (%d %d %s)\n", GREEN_C, ch_r.ssh_answer, ch_r.pcode, ch_r.comment);
    }
}

void getChannel(Server this)
{
    struct channel_data ch_d;
    int size;

    this->server_receive(this, &ch_d, SIZEOF_CH_D);
    size = strlen(ch_d.service_name);
    printf("(%s)\n", ch_d.command);
    fflush(stdout);

    if (strncmp("exec", ch_d.service_name, size) == 0)
    {
        printf("One shot\n");
        oneshotexec(this, ch_d.command);
    }
    else if (strncmp("shell", ch_d.service_name, size) == 0)
    {
        exec_loop(this);
    }
}
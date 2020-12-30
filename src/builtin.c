/**
 * Auteur:                Seddar Naïm
 * Création:              21/11/2020 12:13:21
 * Dernière modification: 30/12/2020 11:55:09
 * Master 1 Informatique
 */

#include "../includes/builtin.h"
#include "../includes/utils.h"
#include "../includes/myls.h"
#include "../includes/myps.h"
#include "../includes/mysh.h"

int mybg(char *var)
{

    int job = -1;
    pid_t pid;

    if (var != NULL)
    {
        if (isalpha(var[0]))
        {
            printf("mybg <job>\n");
            return 2;
        }

        job = atoi(var);

        if (jobs[job].show == 0 || (strcmp(jobs[job].etat, "En cours d'exécution") == 0))
        {
            printf("Job inexistant ou déjà en cours d'exécution\n");
            return 2;
        }

        pid = jobs[job].pid;

        kill(pid, SIGTTIN);
        kill(pid, SIGCONT);

        return 0;
    }
    else if (cmd_pid != -1)
    {
        jobs[nb_jobs].show = 1;
        jobs[nb_jobs].job_id = nb_jobs;
        jobs[nb_jobs].pid = cmd_pid;
        jobs[nb_jobs].etat = "En cours d'exécution";
        memcpy(jobs[nb_jobs].command, prev_cmd, strlen(prev_cmd));

        kill(cmd_pid, SIGTTIN);
        kill(cmd_pid, SIGCONT);

        return 0;
    }
    else
    {
        for (int i = nb_jobs - 1; i >= 0; i--)
        {
            printf("%d %s\n", jobs[i].show, jobs[i].command);
            if (jobs[i].show == 1)
            {
                // job = jobs[i].job_id;
                pid = jobs[i].pid;
                break;
            }
        }

        if (pid == -1)
        {
            printf("Aucun job peut être mit en background\n");
            return 2;
        }
    }

    kill(pid, SIGTTIN);
    kill(pid, SIGCONT);

    return 0;
}

int myfg(char *var)
{
    int job = -1;
    int status;
    pid_t pid;

    if (var != NULL)
    {
        if (isalpha(var[0]))
        {
            printf("myfg <job>\n");
            return 2;
        }

        job = atoi(var);

        if (jobs[job].show == 0)
        {
            printf("Job inexistant\n");
            return 2;
        }
    }
    else
    {
        for (int i = nb_jobs - 1; i >= 0; i--)
        {
            printf("%d %s\n", jobs[i].show, jobs[i].command);
            if (jobs[i].show == 1)
            {
                job = jobs[i].job_id;
                break;
            }
        }

        if (job == -1)
        {
            printf("Aucun job ne peut être mit en foreground\n");
            return 2;
        }
    }

    jobs[job].show = 0;
    pid = jobs[job].pid;

    kill(pid, SIGCONT);
    waitpid(pid, &status, WUNTRACED);

    if (WIFEXITED(status))
    {
        prev_pcode = WEXITSTATUS(status);
        return prev_pcode;
    }

    prev_pcode = -1;

    return prev_pcode;
}

int status()
{
    if (prev_fg_proc == -1)
    {
        printf("Aucune commande n'a encore été exécuté en premier plan\n");
        return 0;
    }
    else
    {
        printf("%d terminé ", prev_fg_proc);
    }

    if (prev_pcode != -1)
    {
        printf("avec comme code de retour %d\n", prev_pcode);
    }
    else
    {
        printf("anormalement\n");
    }

    return 0;
}

int myjobs()
{
    if (nb_jobs == 0)
        return 0;
    for (int i = 0; i < nb_jobs; i++)
    {
        if (jobs[i].show)
            printf("[%d] %d %s %s\n", jobs[i].job_id, jobs[i].pid, jobs[i].etat, jobs[i].command);
    }
    return 0;
}

int clear_prompt()
{
    printf("\33[H\33[J");
    return 0;
}

int cd(char *path)
{
    if (path == NULL)
    {
        path = getenv("HOME");
    }

    if (chdir(path) == ERR)
    {
        perror("cd");
        return 2;
    }

    return 0;
}

int set_var(char *var)
{
    if (var != NULL)
        return putenv(var);
    else
        return 2;
}

int unset_var(char *var)
{
    if (var != NULL)
        return unsetenv(var);
    else
        return 2;
}

int builtin_parser(char **command_line)
{
    remove_whitespaces(command_line[0]);

    if (strncmp(command_line[0], "cd", 2) == 0)
    {
        return cd(command_line[1]);
    }
    else if (strncmp(command_line[0], "exit", 4) == 0)
    {
        exit(0);
    }
    else if (strncmp(command_line[0], "myls", 4) == 0)
    {
        myls(++command_line);
        return 0;
    }
    else if (strncmp(command_line[0], "myps", 4) == 0)
    {
        return myps();
    }
    else if (strncmp(command_line[0], "set", 3) == 0)
    {
        return set_var(command_line[1]);
    }
    else if (strncmp(command_line[0], "unset", 5) == 0)
    {
        return unset_var(command_line[1]);
    }
    else if (strncmp(command_line[0], "clear", 5) == 0)
    {
        return clear_prompt();
    }
    else if (strncmp(command_line[0], "myjobs", 6) == 0)
    {
        return myjobs();
    }
    else if (strncmp(command_line[0], "status", 6) == 0)
    {
        return status();
    }
    else if (strncmp(command_line[0], "myfg", 4) == 0)
    {
        return myfg(command_line[1]);
    }
    else if (strncmp(command_line[0], "mybg", 4) == 0)
    {
        return mybg(command_line[1]);
    }

    return -2;
}

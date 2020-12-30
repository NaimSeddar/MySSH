/**
 * Auteur:                Seddar Naïm
 * Création:              09/11/2020 13:34:02
 * Dernière modification: 30/12/2020 12:29:59
 * Master 1 Informatique
 */

#include "../includes/utils.h"

char **str_split(char *str, const char delimiter)
{
    char **result = 0;
    size_t count = 0;
    char *tmp = str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = delimiter;
    delim[1] = 0;

    while (*tmp)
    {
        if (delimiter == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (str + strlen(str) - 1);

    count++;

    result = malloc(sizeof(char *) * count);

    if (result)
    {
        size_t idx = 0;
        char *token = strtok(str, delim);

        while (token)
        {
            if (idx >= count)
                exit(-1);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        *(result + idx) = NULL;
    }

    return result;
}

int occu(char *str, const char *needle)
{
    char *pos_occu;
    int index = 0;
    int pos = 0;

    while ((pos_occu = strstr(str + index, needle)) != NULL)
    {
        index = (pos_occu - str) + 1;
        pos++;
    }
    return pos;
}

char **str_splitv2(char *str, const char *delimiter)
{
    char **result = 0;
    int count = 0;
    int len_del = strlen(delimiter);
    int len_str = strlen(str);
    char *buf;
    int len;
    int i = 0;

    count = occu(str, delimiter);

    if (count > 0)
    {
        result = malloc(sizeof(char *) * (count + 2));
        while ((buf = strstr(str, delimiter)) != NULL)
        {
            len = buf - str;
            result[i] = malloc(sizeof(char) * len);
            strncpy(result[i], str, len);
            str += len + len_del;
            i++;
        }
        len = len_str - strlen(str);
        result[i] = malloc(sizeof(char) * len);
        strcpy(result[i], str);
        result[++i] = NULL;
    }
    else
    {
        result = malloc(sizeof(char *));
        result[i] = malloc(sizeof(char) * len_str);
        strncpy(result[0], str, len_str);
    }

    return result;
}

void remove_whitespaces(char *str)
{
    const char *d = str;
    do
    {
        while (*d == ' ')
        {
            ++d;
        }
    } while ((*str++ = *d++));
}

char *file_to_string(char *filename)
{
    int f = open(filename, O_RDONLY);
    char *buffer = (char *)malloc(BUFFER_SIZE_MALLOC);

    for (int i = 0; read(f, buffer + i, 1) > 0; i++)
    {
        if (i >= BUFFER_SIZE_MALLOC)
        {
            buffer = (char *)realloc(buffer, sizeof(buffer) + BUFFER_SIZE_MALLOC);
        }
    }

    close(f);

    return buffer;
}
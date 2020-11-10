#include "utils.h"

/*
 * Better strtok
 */
char **str_split(char *str, const char delimiter)
{
    char **result = 0;
    size_t count = 0;
    char *tmp = str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = delimiter;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (delimiter == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (str + strlen(str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
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
        *(result + idx) = 0;
    }

    return result;
}

/*
 * File into string
 */

char *file_to_string(char *filename)
{
    int f = open(filename, O_RDONLY);
    char *buffer = (char *)malloc(BUFFER_SIZE);

    for (int i = 0; read(f, buffer + i, 1) > 0; i++)
    {
        if (i >= BUFFER_SIZE)
        {
            buffer = (char *)realloc(buffer, sizeof(buffer) + BUFFER_SIZE);
        }
    }

    close(f);

    return buffer;
}
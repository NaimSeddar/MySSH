#include "../includes/colors.h"

void grayBG()
{
    printf("\033[40m");
}

void whiteBG()
{
    printf("\033[47m");
}

void writein(const char *color)
{
    if (write(STDOUT_FILENO, color, sizeof(color)) == -1)
        perror("Write"), exit(1);
}
#include "colors.h"

void red(char *m)
{
    printf("%s%s%s\n", RED_C, m, RESET_C);
}
void green(char *m)
{
    printf("%s%s%s\n", GREEN_C, m, RESET_C);
}

void blue(char *m)
{
    printf("%s%s%s\n", BLUE_C, m, RESET_C);
}

void white(char *m)
{
    printf("%s%s%s\n", WHITE_C, m, RESET_C);
}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

#define RED(m) "\033[01;31m" m "\033[0m"
#define GREEN(m) "\033[01;32m" m "\033[0m"

int main(int argv, char *argc[])
{
    // char* getcwd(char *buf, size_t size)
    // char* getcwd(char *buf)

    char buffer[1024];
    printf("%s\n", getcwd(buffer, sizeof(buffer)));

    return 0;
}
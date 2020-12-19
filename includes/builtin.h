#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int cd(char *path);

int builtin_parser(char **command_line);
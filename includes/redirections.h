#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int stdout_to_fic(char *command, int flags);

int stderr_to_fic(char *command, int flags);

int stderr_and_stdout(char *command, int flags);
#include <stdio.h>

// [COLOR]_C
#define RED_C "\033[1;31m"
#define GREEN_C "\033[1;32m"
#define BLUE_C "\033[1;34m"
#define WHITE_C "\033[1;39m"
#define RESET_C "\033[0m"

// COLOR(message)
void RED(char *m);
void GREEN(char *m);
void BLUE(char *m);
void WHITE(char *m);
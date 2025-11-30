// colour.c

#include "../include/colour.h"

void colour_print_set(Col c)
{
        printf("\x1b[48;2;%d;%d;%dm", c.r, c.g, c.b);
}

void colour_print_reset()
{
        printf("\x1b[0m");
}
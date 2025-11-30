//time.c

#include "../include/time.h"

void sleep_seconds(float seconds)
{
        usleep(1000000 * seconds); 
}
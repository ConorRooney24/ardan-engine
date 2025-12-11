// rand.c

#include "rand.h"

void rand_seed()
{
        srand(time(NULL));
}

int rand_int(int a, int b) 
{
        if (a > b) 
        {
                int temp = a;
                a = b;
                b = temp;
        }

        return a + rand() % (b - a + 1);
}

float rand_float(float a, float b)
{
        if (a > b) 
        { 
                float temp = a; 
                a = b; 
                b = temp; 
        }

        return a + (b - a) * ( rand() / (float)RAND_MAX );
}
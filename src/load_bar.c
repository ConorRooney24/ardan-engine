// load_bar.c

#include "../include/load_bar.h"
#include <stdio.h>
#include <math.h>


void load_bar_print(float current_value, float completion_value, int width)
{
        float ratio = current_value / completion_value;

        if (ratio < 0.0f) 
        {
                ratio = 0.0f;
        }
        else if (ratio > 1.0f)
        {
                ratio = 1.0f;
        }

        int chars_to_print = (int)floor(ratio * width);

        printf("[");
        for (int i = 0; i < chars_to_print; i++) 
        {
                printf("#");
        }
        for (int i = chars_to_print; i < width; i++) 
        {
                printf(".");
        }
        printf("]\n");
}
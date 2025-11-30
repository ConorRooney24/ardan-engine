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

Col colour_skew(Col c, int max_ammount)
{
        for (int i = 0; i < max_ammount; i++)
        {
                if (c.r >= 255) // We must subtract
                {
                        c.r = 255;
                        c.r -= rand_int(0, max_ammount);
                }
                else // Randomly decide weather or not to subtract or add
                {
                        if (rand_int(0, 1))
                        {
                                c.r -= rand_int(0, max_ammount);
                        }
                        else
                        {
                                c.r += rand_int(0, max_ammount);
                        }
                }

                if (c.g >= 255)
                {
                        c.g = 255;
                        c.g -= rand_int(0, max_ammount);
                }
                else
                {
                        if (rand_int(0, 1))
                        {
                                c.g -= rand_int(0, max_ammount);
                        }
                        else
                        {
                                c.g += rand_int(0, max_ammount);
                        }
                }

                if (c.b >= 255)
                {
                        c.b = 255;
                        c.b -= rand_int(0, max_ammount);
                }
                else
                {
                        if (rand_int(0, 1))
                        {
                                c.b -= rand_int(0, max_ammount);
                        }
                        else
                        {
                                c.b += rand_int(0, max_ammount);
                        }
                }
        }
}
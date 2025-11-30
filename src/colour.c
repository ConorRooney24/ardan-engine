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
                        c.r -= rand_int(0, 1);
                }
                else if (c.r <= 0) // We must add
                {
                        c.r = 0;
                        c.r += rand_int(0, 1);
                }
                else // Randomly decide weather or not to subtract or add
                {
                        if (rand_int(0, 1))
                        {
                                c.r -= rand_int(0, 1);
                        }
                        else
                        {
                                c.r += rand_int(0, 1);
                        }
                }

                if (c.g >= 255) // We must subtract
                {
                        c.g = 255;
                        c.g -= rand_int(0, 1);
                }
                else if (c.g <= 0) // We must add
                {
                        c.g = 0;
                        c.g += rand_int(0, 1);
                }
                else // Randomly decide weather or not to subtract or add
                {
                        if (rand_int(0, 1))
                        {
                                c.g -= rand_int(0, 1);
                        }
                        else
                        {
                                c.g += rand_int(0, 1);
                        }
                }

                if (c.b >= 255) // We must subtract
                {
                        c.b = 255;
                        c.b -= rand_int(0, 1);
                }
                else if (c.b <= 0) // We must add
                {
                        c.b = 0;
                        c.b += rand_int(0, 1);
                }
                else // Randomly decide weather or not to subtract or add
                {
                        if (rand_int(0, 1))
                        {
                                c.b -= rand_int(0, 1);
                        }
                        else
                        {
                                c.b += rand_int(0, 1);
                        }
                }
        }

        return c;
}
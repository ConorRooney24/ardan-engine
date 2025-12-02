// col.c

#include "../include/col.h"

void col_print_set(Col c)
{
        printf("\x1b[48;2;%d;%d;%dm", c.r, c.g, c.b);
}

void col_print_reset()
{
        printf("\x1b[0m");
}

Col col_skew(Col c, int max_amount)
{
        for (int i = 0; i < max_amount; i++)
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

Col col_blend(Col a, Col b, float ratio)
{
        if (!(ratio >= 0.0f && ratio <= 1.0f))
        {
                col_print_set(COL_RED);
                printf("Error: Ratio must be between 0.0 and 1.0 inclusive");
                col_print_reset();
                printf("\n");
                return COL_UNDEFINED;
        }
        
        Col c;
        float amount_a, amount_b;

        amount_a = 1 - ratio;
        amount_b = ratio;

        c.r = (( (float)a.r ) * amount_a) + (( (float)b.r ) * amount_b);
        c.g = (( (float)a.g ) * amount_a) + (( (float)b.g ) * amount_b);
        c.b = (( (float)a.b ) * amount_a) + (( (float)b.b ) * amount_b);

        return c;
}
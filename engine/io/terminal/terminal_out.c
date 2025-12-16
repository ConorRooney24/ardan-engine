// terminal_out.c

#include "terminal_out.h"
#include "../../util/util_time.h"

void terminal_out_print(Buffer *buffer, char character)
{
        //printf("\033[2J"); //cls
        printf("\033[H"); // cursor to top left

        for (int y = 0; y < buffer->res_y; y++)
        {
                //sleep_ns(5000000);
                for (int x = 0; x < buffer->res_x; x++)
                {
                        //sleep_ms(1);
                        col_print_set(buffer->pixels[y][x]);
                        printf("%c%c", character, character);
                        
                }
                col_print_reset();
                printf("\n");

        }
}

// buffer.c

#include <stdlib.h>

#include "buffer.h"

Buffer buffer_init(int res_x, int res_y)
{
        Buffer buffer;
        buffer.res_x = res_x;
        buffer.res_y = res_y;

        // malloc
        buffer.pixels = (Col**)malloc(res_y * sizeof(Col*));

        for (int i = 0; i < res_y; i++)
        {
                buffer.pixels[i] = malloc(res_x * sizeof(Col));
        }

        // Set colours
        for (int y = 0; y < res_y; y++)
        {
                for (int x = 0; x < res_x; x++)
                {
                        buffer.pixels[y][x].r = 255;
                        buffer.pixels[y][x].g = 255;
                        buffer.pixels[y][x].b = 255;
                }
        }

        return buffer;
}

void buffer_destroy(Buffer *buffer)
{
        for (int y = 0; y < buffer->res_y; y++)
        {
                free(buffer->pixels[y]);
        }
        
        free(buffer->pixels);

        buffer->pixels = NULL;
        
        buffer->res_x = 0;
        buffer->res_y = 0;  
}

// renderer.c

#include <stdio.h>
#include <stdlib.h>
#include "../include/renderer.h"
#include "../include/config.h"

// Pixel buffer[DISPLAY_HEIGHT_MAX][DISPLAY_WIDTH_MAX];

Buffer buffer_init(int width, int height)
{
        Buffer buffer;
        buffer.width = width;
        buffer.height = height;

        buffer.pixels = (Pixel**)malloc(height * sizeof(Pixel*));

        for (int i = 0; i < height; i++)
        {
                buffer.pixels[i] = malloc(width * sizeof(Pixel));
        }
        
        

        for (int y = 0; y < height; y++)
        {
                for (int x = 0; x < width; x++)
                {
                        buffer.pixels[y][x].character = PIXEL_CHARACTER_DEFAULT;
                        buffer.pixels[y][x].colour.r = PIXEL_COLOUR_R_DEFAULT;
                        buffer.pixels[y][x].colour.g = PIXEL_COLOUR_G_DEFAULT;
                        buffer.pixels[y][x].colour.b = PIXEL_COLOUR_B_DEFAULT;
                }
        }

        return buffer;
}

void buffer_destroy(Buffer* buffer)
{
        for (int y = 0; y < buffer->height; y++)
        {
                free(buffer->pixels[y]);
        }

        free(buffer->pixels);
        buffer->pixels = NULL;
        buffer->width = 0;
        buffer->height = 0;
}

void buffer_print(Buffer *buffer)
{
        for (int y = 0; y < buffer->height; y++)
        {
                for (int x = 0; x < buffer->width; x++)
                {
                        printf("\x1b[48;2;%d;%d;%dm%c%c", buffer->pixels[y][x].colour.r, buffer->pixels[y][x].colour.g, buffer->pixels[y][x].colour.b, buffer->pixels[y][x].character, buffer->pixels[y][x].character);
                }
                printf("\n");
        }
        printf("\x1b[0m"); // Colour reset
}
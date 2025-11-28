// renderer.c

#include <stdio.h>
#include "../include/renderer.h"
#include "../include/config.h"

Pixel buffer[DISPLAY_HEIGHT_MAX][DISPLAY_WIDTH_MAX];

void buffer_init()
{
        for (int y = 0; y < config.display_height; y++)
        {
                for (int x = 0; x < config.display_width; x++)
                {
                        buffer[y][x].character = PIXEL_CHARACTER_DEFAULT;
                        buffer[y][x].colour.r = PIXEL_COLOUR_R_DEFAULT;
                        buffer[y][x].colour.g = PIXEL_COLOUR_G_DEFAULT;
                        buffer[y][x].colour.b = PIXEL_COLOUR_B_DEFAULT;
                }
        }
}

void buffer_print()
{
        for (int y = 0; y < config.display_height; y++)
        {
                for (int x = 0; x < config.display_width; x++)
                {
                        printf("\x1b[48;2;%d;%d;%dm%c%c", buffer[y][x].colour.r, buffer[y][x].colour.g, buffer[y][x].colour.b, buffer[y][x].character, buffer[y][x].character);
                }
                printf("\n");
        }
        printf("\x1b[0m"); // Colour reset
}
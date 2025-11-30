// renderer.c

#include <stdio.h>
#include <stdlib.h>
#include "../include/renderer.h"
#include "../include/config.h"

// Pixel buffer[DISPLAY_HEIGHT_MAX][DISPLAY_WIDTH_MAX];

Buffer buffer_init(int resolution_x, int resolution_y)
{
        Buffer buffer;
        buffer.resolution_x = resolution_x;
        buffer.resolution_y = resolution_y;

        buffer.rays = (Ray**)malloc(resolution_y * sizeof(Ray*));
        buffer.pixels = (Pixel**)malloc(resolution_y * sizeof(Pixel*));

        for (int i = 0; i < resolution_y; i++)
        {
                buffer.rays[i] = malloc(resolution_x * sizeof(Ray));
                buffer.pixels[i] = malloc(resolution_x * sizeof(Pixel));
        }
        
        

        for (int y = 0; y < resolution_y; y++)
        {
                for (int x = 0; x < resolution_x; x++)
                {
                        buffer.rays[y][x] = ray_init((Vec3){0,0,0}, (Vec2){0,0});

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
        for (int y = 0; y < buffer->resolution_y; y++)
        {
                free(buffer->rays[y]);
                free(buffer->pixels[y]);
        }

        free(buffer->rays);
        free(buffer->pixels);

        buffer->rays = NULL;
        buffer->pixels = NULL;
        
        buffer->resolution_x = 0;
        buffer->resolution_y = 0;
}

void buffer_translate_ray_data(Buffer *buffer)
{
        for (int y = 0; y < buffer->resolution_y; y++)
        {
                for (int x = 0; x < buffer->resolution_x; x++)
                {
                        if (buffer->rays[y][x].collided)
                        {
                                buffer->pixels[y][x].colour = buffer->rays[y][x].collided_material.col;
                        }
                        else
                        {
                                buffer->pixels[y][x].colour = materials[MAT_AIR].col; // If the ray has not collided (meaning it went out of bounds) we set it to the air colour to avoid black patches.
                        }
                }
                
        }
        
}

void buffer_print(Buffer *buffer)
{
        printf("\033[H");
        for (int y = 0; y < buffer->resolution_y; y++)
        {
                for (int x = 0; x < buffer->resolution_x; x++)
                {
                        colour_print_set(buffer->pixels[y][x].colour);
                        printf("%c%c", buffer->pixels[y][x].character, buffer->pixels[y][x].character);
                }
                printf("\x1b[0m");
                printf("\n");
        }
}

void buffer_print_ray_material_id(Buffer *buffer)
{
        for (int y = 0; y < buffer->resolution_y; y++)
        {
                for (int x = 0; x < buffer->resolution_x; x++)
                {
                        printf("%d, ", buffer->rays[y][x].collided_material.mat_id);
                }
                printf("\n");
        }     
}
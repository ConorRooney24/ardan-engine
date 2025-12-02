// renderer.c

#include <stdio.h>
#include <stdlib.h>
#include "../include/renderer.h"
#include "../include/config.h"

// Pixel buffer[DISPLAY_HEIGHT_MAX][DISPLAY_WIDTH_MAX];

Buffer buffer_init(int res_x, int res_y)
{
        Buffer buffer;
        buffer.res_x = res_x;
        buffer.res_y = res_y;

        buffer.rays = (Ray**)malloc(res_y * sizeof(Ray*));
        buffer.pixels = (Pixel**)malloc(res_y * sizeof(Pixel*));

        for (int i = 0; i < res_y; i++)
        {
                buffer.rays[i] = malloc(res_x * sizeof(Ray));
                buffer.pixels[i] = malloc(res_x * sizeof(Pixel));
        }
        
        

        for (int y = 0; y < res_y; y++)
        {
                for (int x = 0; x < res_x; x++)
                {
                        buffer.rays[y][x] = ray_init((Vec3){0,0,0}, (Vec2){0,0});

                        buffer.pixels[y][x].character = PIXEL_CHARACTER_DEFAULT;
                        buffer.pixels[y][x].col.r = PIXEL_COL_R_DEFAULT;
                        buffer.pixels[y][x].col.g = PIXEL_COL_G_DEFAULT;
                        buffer.pixels[y][x].col.b = PIXEL_COL_B_DEFAULT;
                }
        }

        return buffer;
}

void buffer_destroy(Buffer* buffer)
{
        for (int y = 0; y < buffer->res_y; y++)
        {
                free(buffer->rays[y]);
                free(buffer->pixels[y]);
        }

        free(buffer->rays);
        free(buffer->pixels);

        buffer->rays = NULL;
        buffer->pixels = NULL;
        
        buffer->res_x = 0;
        buffer->res_y = 0;
}

void buffer_translate_ray_data(Buffer *buffer)
{
        for (int y = 0; y < buffer->res_y; y++)
        {
                for (int x = 0; x < buffer->res_x; x++)
                {
                        if (buffer->rays[y][x].collided)
                        {
                                buffer->pixels[y][x].col = buffer->rays[y][x].collided_material.col;
                        }
                        else
                        {
                                buffer->pixels[y][x].col = materials[MAT_AIR].col; // If the ray has not collided (meaning it went out of bounds) we set it to the air colour to avoid black patches.
                        }
                }
                
        }
        
}

void buffer_print(Buffer *buffer)
{
        printf("\033[H");
        for (int y = 0; y < buffer->res_y; y++)
        {
                for (int x = 0; x < buffer->res_x; x++)
                {
                        col_print_set(buffer->pixels[y][x].col);
                        printf("%c%c", buffer->pixels[y][x].character, buffer->pixels[y][x].character);
                }
                printf("\x1b[0m");
                printf("\n");
        }
}

void buffer_print_ray_material_id(Buffer *buffer)
{
        for (int y = 0; y < buffer->res_y; y++)
        {
                for (int x = 0; x < buffer->res_x; x++)
                {
                        printf("%d, ", buffer->rays[y][x].collided_material.mat_id);
                }
                printf("\n");
        }     
}
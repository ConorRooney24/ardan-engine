// ray_translator.c

#include <math.h>
#include <stdlib.h>

#include "ray_translator.h"

RayBuffer ray_buffer_init(int res_x, int res_y)
{
        RayBuffer buffer;
        buffer.res_x = res_x;
        buffer.res_y = res_y;

        // malloc
        buffer.rays = (Ray**)malloc(res_y * sizeof(Ray*));

        for (int i = 0; i < res_y; i++)
        {
                buffer.rays[i] = malloc(res_x * sizeof(Ray));
        }

        // init rays
        for (int y = 0; y < res_y; y++)
        {
                for (int x = 0; x < res_x; x++)
                {
                        buffer.rays[y][x] = ray_init((Vec3){0, 0, 0}, (Vec2){0, 0});
                }
        }

        return buffer;
}

void ray_buffer_destroy(RayBuffer *buffer)
{
        for (int y = 0; y < buffer->res_y; y++)
        {
                free(buffer->rays[y]);
        }
        
        free(buffer->rays);

        buffer->rays = NULL;
        
        buffer->res_x = 0;
        buffer->res_y = 0;  
}

void ray_buffer_translate(RayBuffer *ray_buffer, Buffer *buffer) // Translates the ray data into colour info and stores in the buffer
{
        if (ray_buffer->res_x != buffer->res_x || ray_buffer->res_y != buffer->res_y)
        {
                col_print_set(COL_RED);
                printf("Error: Ray Buffer resolution does not match pixel buffer resolution.");
                col_print_reset();
                printf("\n");

                return;
        }

        int res_x = ray_buffer->res_x;
        int res_y = ray_buffer->res_y;
        
        for (int y = 0; y < res_y; y++)
        {
                for (int x = 0; x < res_x; x++)
                {
                        if (ray_buffer->rays[y][x].collided)
                        {
                                int face_hit;
                                float U, V;

                                face_hit = ray_buffer->rays[y][x].face_hit;

                                U = ray_buffer->rays[y][x].tex_u;
                                V = ray_buffer->rays[y][x].tex_v;

                                // Now we translate UV into texel coords and set the colour
                                int tx = (int)floor(U * TEXT_RES_X); // Texel x
                                int ty = (int)floor(V * TEXT_RES_Y); // Texel y

                                if (tx < 0)
                                {
                                        tx = 0;
                                }
                                else if (tx >= TEXT_RES_X)
                                {
                                        tx = TEXT_RES_X - 1;
                                }

                                if (ty < 0)
                                {
                                        ty = 0;
                                }
                                else if (ty >= TEXT_RES_Y)
                                {
                                        ty = TEXT_RES_Y - 1;
                                }

                                buffer->pixels[y][x] = ray_buffer->rays[y][x].collided_vox.text->col_map[face_hit][ty][tx];
                                
                        }
                        else
                        {
                                buffer->pixels[y][x] = (Col){182, 224, 240}; // If the ray has not collided (meaning it went out of bounds) we set it to blue
                        }
                }
                
        }
}

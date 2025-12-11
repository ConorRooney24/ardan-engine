// renderer.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "renderer.h"
#include "config.h"
#include "../util/util_time.h"

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
                                int face_hit;

                                /* OLD CODE - FOR DDA THIS IS DONE IN THE RAY_CAST FUNC
                                // Start by calculating which face of the voxel was hit.
                                

                                int last_alive_pos_x = (int)floorf(buffer->rays[y][x].pos_last_alive.x);
                                int last_alive_pos_y = (int)floorf(buffer->rays[y][x].pos_last_alive.y);
                                int last_alive_pos_z = (int)floorf(buffer->rays[y][x].pos_last_alive.z);

                                int collision_pos_x = (int)floorf(buffer->rays[y][x].collision_pos.x);
                                int collision_pos_y = (int)floorf(buffer->rays[y][x].collision_pos.y);
                                int collision_pos_z = (int)floorf(buffer->rays[y][x].collision_pos.z);

                                int pos_delta_x = abs(collision_pos_x - last_alive_pos_x);
                                int pos_delta_y = abs(collision_pos_y - last_alive_pos_y);
                                int pos_delta_z = abs(collision_pos_z - last_alive_pos_z);

                                if (pos_delta_x >= pos_delta_y && pos_delta_x >= pos_delta_z)
                                {
                                        if (collision_pos_x > last_alive_pos_x)
                                        {
                                                face_hit = VOX_FACE_X_PLUS;
                                        }
                                        else
                                        {
                                                face_hit = VOX_FACE_X_MINUS;
                                        }
                                }
                                else if (pos_delta_y >= pos_delta_x && pos_delta_y >= pos_delta_z)
                                {
                                        if (collision_pos_y > last_alive_pos_y)
                                        {
                                                face_hit = VOX_FACE_Y_PLUS;
                                        }
                                        else
                                        {
                                                face_hit = VOX_FACE_Y_MINUS;
                                        }  
                                }
                                else
                                {
                                        if (collision_pos_z > last_alive_pos_z)
                                        {
                                                face_hit = VOX_FACE_Z_PLUS;
                                        }       
                                        else
                                        {
                                                face_hit = VOX_FACE_Z_MINUS;
                                        }
                                } */

                                face_hit = buffer->rays[y][x].face_hit;

                                // Now that we have the face, we calculate the UV
                                float U, V;
                                /*
                                if (face_hit == VOX_FACE_X_MINUS)
                                {
                                        U = buffer->rays[y][x].pos_last_alive.z - floor(buffer->rays[y][x].pos_last_alive.z);
                                        V = buffer->rays[y][x].pos_last_alive.y - floor(buffer->rays[y][x].pos_last_alive.y);
                                }
                                else if (face_hit == VOX_FACE_X_PLUS)
                                {
                                        // Flip U horizontally
                                        U = 1.0f - (buffer->rays[y][x].pos_last_alive.z - floor(buffer->rays[y][x].pos_last_alive.z));
                                        V = buffer->rays[y][x].pos_last_alive.y - floor(buffer->rays[y][x].pos_last_alive.y);
                                }
                                else if (face_hit == VOX_FACE_Y_MINUS)
                                {
                                        U = buffer->rays[y][x].pos_last_alive.x - floor(buffer->rays[y][x].pos_last_alive.x);
                                        V = buffer->rays[y][x].pos_last_alive.z - floor(buffer->rays[y][x].pos_last_alive.z);
                                }
                                else if (face_hit == VOX_FACE_Y_PLUS)
                                {
                                        U = buffer->rays[y][x].pos_last_alive.x - floor(buffer->rays[y][x].pos_last_alive.x);
                                        V = 1.0f - (buffer->rays[y][x].pos_last_alive.z - floor(buffer->rays[y][x].pos_last_alive.z));
                                }
                                else if (face_hit == VOX_FACE_Z_MINUS)
                                {
                                        U = buffer->rays[y][x].pos_last_alive.x - floor(buffer->rays[y][x].pos_last_alive.x);
                                        V = buffer->rays[y][x].pos_last_alive.y - floor(buffer->rays[y][x].pos_last_alive.y);
                                }
                                else // VOX_FACE_Z_PLUS
                                {
                                        U = 1.0f - (buffer->rays[y][x].pos_last_alive.x - floor(buffer->rays[y][x].pos_last_alive.x));
                                        V = buffer->rays[y][x].pos_last_alive.y - floor(buffer->rays[y][x].pos_last_alive.y);
                                }*/

                                U = buffer->rays[y][x].tex_u;
                                V = buffer->rays[y][x].tex_v;

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

                                //buffer->pixels[y][x].col = buffer->rays[y][x].collided_vox.text.col_map[face_hit][ty][tx];
                                buffer->pixels[y][x].col = buffer->rays[y][x].collided_vox.text->col_map[face_hit][ty][tx];
                                //printf("tx: %d, ty: %d, U: %.2f, V: %.2f\n", tx, ty, U, V);
                                
                        }
                        else
                        {
                                buffer->pixels[y][x].col = (Col){182, 224, 240}; // If the ray has not collided (meaning it went out of bounds) we set it to blue
                        }
                }
                
        }
        
}

void buffer_print(Buffer *buffer)
{
        printf("\033[H"); // cursor to top left

        for (int y = 0; y < buffer->res_y; y++)
        {
                //sleep_ns(1000000);
                for (int x = 0; x < buffer->res_x; x++)
                {
                        col_print_set(buffer->pixels[y][x].col);
                        printf("%c%c", buffer->pixels[y][x].character, buffer->pixels[y][x].character);
                        
                }
                col_print_reset();
                printf("\n");

        }
}

void buffer_print_ray_material_id(Buffer *buffer)
{
        for (int y = 0; y < buffer->res_y; y++)
        {
                for (int x = 0; x < buffer->res_x; x++)
                {
                        printf("%d, ", buffer->rays[y][x].collided_vox.mat->mat_id);
                }
                printf("\n");
        }     
}
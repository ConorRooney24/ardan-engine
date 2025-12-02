#include <stdio.h>
#include <math.h>

#include "include/map.h"
#include "include/material.h"
#include "include/config.h"
#include "include/renderer.h"
#include "include/camera.h"
#include "include/trig.h"
#include "include/util_time.h"
#include "include/input.h"

#define RES_X 100
#define RES_Y 50
#define MAP_X 100
#define MAP_Y 50
#define MAP_Z 100

int main(void)
{
        config.max_ray_distance = 400;
        config.ray_step_scalar_coarse = 1.0f;
        config.ray_step_scalar_fine = 0.03f;


        materials_init(materials);

        Buffer buffer = buffer_init(RES_X, RES_Y);
        Map map = map_init((Vec3i){MAP_X,MAP_Y,MAP_Z});

        for (int x = 0; x < map.size.x; x++)
        {
                for (int y = 0; y < map.size.y; y++)
                {
                        for (int z = 0; z < map.size.z; z++)
                        {
                                //map.grid[x][y][z].mat_id = MAT_AIR;
                                //map.grid[x][y][z].mat_id = MAT_BLOCK_GREEN;
                                if (x == map.size.x-1 || x == 0)
                                {
                                        map.grid[x][y][z] = materials[MAT_BLOCK_RED];
                                }
                                else if (y == map.size.y-1 || y == 0)
                                {
                                        map.grid[x][y][z] = materials[MAT_BLOCK_GREEN];
                                }
                                else if (z == map.size.z-1 || z == 0)
                                {
                                        map.grid[x][y][z] = materials[MAT_BLOCK_BLUE];
                                }
                                else
                                {
                                        map.grid[x][y][z] = materials[MAT_AIR];
                                }
                                
                        }
                }
        }

        Vec3 pos;
        Vec2 dir_deg;
        pos.x = MAP_X/2;
        pos.y = 2;
        pos.z = MAP_Z/2;
        dir_deg.x = 0;
        dir_deg.y = 0;

        Camera c = camera_init(RES_X, RES_Y, 90, pos, dir_deg);
        camera_calculate_vFOV(&c);

        printf("\033[2J");

        TimeStamp frame_start;
        TimeStamp frame_end;

        input_init();
        bool running = true;
        while (running)
        {
                frame_start = record_time();
                input_update();

                if(input_key_pressed('0'))
                {
                        running = false;
                }

                if(input_key_pressed('w'))
                {
                        c.pos = vec3_add(c.pos, c.dir);
                }
                if(input_key_pressed('a'))
                {
                        c.dir_deg.x++;
                }
                if(input_key_pressed('s'))
                {
                        c.pos = vec3_subtract(c.pos, c.dir);
                }
                if(input_key_pressed('d'))
                {
                        c.dir_deg.x++;
                }

                if(input_key_pressed('i'))
                {
                        c.dir_deg.x+=2;
                }
                if(input_key_pressed('k'))
                {
                        c.dir_deg.x-=2;
                }
                if(input_key_pressed('l'))
                {
                        c.dir_deg.y+=2;
                }
                if(input_key_pressed('j'))
                {
                        c.dir_deg.y-=2;
                }


                
                c.dir_rad.x = deg_to_rad(c.dir_deg.x);
                c.dir_rad.y = deg_to_rad(c.dir_deg.y);
                c.dir = dir_rad_to_dir(c.dir_rad);
                c.dir = vec3_normalize(c.dir);

                camera_rays_reset(&c);
                camera_calculate_ray_trajectories(&c);
                capture_and_pass(&c, &buffer, &map);
                buffer_translate_ray_data(&buffer);
                buffer_print(&buffer);
                //sleep_ns(20000000);

                frame_end = record_time();
                printf("FPS: %.1f", time_frame_time_to_fps(time_diff(frame_start, frame_end)));
        }

        input_restore();
        
        printf("\033[2J");

        camera_destroy(&c);
        buffer_destroy(&buffer);

        return 0;
}
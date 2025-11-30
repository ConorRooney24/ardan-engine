#include <stdio.h>
#include <math.h>

#include "include/map.h"
#include "include/material.h"
#include "include/config.h"
#include "include/renderer.h"
#include "include/camera.h"
#include "include/trig.h"
#include "include/util_time.h"

#define RES_X 16*16
#define RES_Y 9*16
#define MAP_X 50
#define MAP_Y 50
#define MAP_Z 100

int main(void)
{
        config.max_ray_distance = 200;
        config.max_ray_step = 1;
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
                                if (x == map.size.x-1)
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
        pos.y = MAP_Y/2;
        pos.z = MAP_Z/2;
        dir_deg.x = 0;
        dir_deg.y = 0;

        Camera c = camera_init(RES_X, RES_Y, 90, pos, dir_deg);
        camera_calculate_vFOV(&c);

        TimeStamp a[360];
        TimeStamp b[360];
        float delta_s[360];
        double total_delta_s = 0;
        float average_frametime_seconds;

        printf("\033[2J"); // clear terminal before render TODO, make a program controller with init and end functions and stuff, also put destroyers in there

        for (int i = 0; i < 360; i++)
        {
                a[i] = record_time();
                c.direction_deg.y = i;
                c.direction_deg.x = sin(i*0.01)*10;
                c.direction_rad.x = deg_to_rad(c.direction_deg.x);
                c.direction_rad.y = deg_to_rad(c.direction_deg.y);
                c.direction = direction_rad_to_direction(c.direction_rad);
                c.direction = vec3_normalize(c.direction);
                camera_rays_reset(&c);
                camera_calculate_ray_trajectories(&c);
                capture_and_pass(&c, &buffer, &map);
                buffer_translate_ray_data(&buffer);
                buffer_print(&buffer);
                b[i] = record_time();
        }

        for (int i = 0; i < 360; i++)
        {
                delta_s[i] = timestamp_to_s(time_diff(a[i], b[i]));
        }

        for (int i = 0; i < 360; i++)
        {
                total_delta_s += (double)delta_s[i];
        }

        average_frametime_seconds = total_delta_s / 360;
        
        printf("\033[2J");

        camera_destroy(&c);
        buffer_destroy(&buffer);

        printf("\n\n\nAverage Frametime (s): %f\n\n\n", average_frametime_seconds);

        return 0;
}
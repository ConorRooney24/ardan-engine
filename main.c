#include <stdio.h>
#include <math.h>

#include "include/map.h"
#include "include/material.h"
#include "include/config.h"
#include "include/renderer.h"
#include "include/cam.h"
#include "include/trig.h"
#include "include/util_time.h"
#include "include/input.h"

#define RES_X 920/4
#define RES_Y 480/4
#define MAP_X 5
#define MAP_Y 5
#define MAP_Z 5
#define FRAMETIME_MIN 0.0f

int main(void)
{
        config.max_ray_distance = 300;
        config.ray_step_scalar_coarse = 1.0f;
        config.ray_step_scalar_fine = 0.01f;
        config.raycast_multithreading = true;

        materials_init(materials);
        text_init();
        map_voxs_init();

        Buffer buffer = buffer_init(RES_X, RES_Y);
        Map map = map_init((Vec3i){MAP_X,MAP_Y,MAP_Z});
        map_load_file(&map, "res/map/MAP_TEST.mapa");

        /*
        for (int x = 0; x < map.size.x; x++)
        {
                for (int y = 0; y < map.size.y; y++)
                {
                        for (int z = 0; z < map.size.z; z++)
                        {
                                if (x == map.size.x-1 || x == 0)
                                {
                                        map.vox[x][y][z] = voxs[VOX_BRICK_WALL_RED];
                                }
                                else if (y == 0)
                                {
                                        map.vox[x][y][z] = voxs[VOX_FLOOR_BOARDS_DIRTY_YELLOW];
                                }
                                else if (y == map.size.y - 1)
                                {
                                        map.vox[x][y][z] = voxs[VOX_PAINT_BEIGE];
                                }
                                else if (z == map.size.z-1 || z == 0)
                                {
                                        map.vox[x][y][z] = voxs[VOX_BRICK_WALL_RED];
                                }
                                else
                                {
                                        map.vox[x][y][z] = voxs[VOX_AIR];
                                }
                        }
                }
        }

        for (int y = 0; y < map.size.y; y++)
        {
                map.vox[(int)floor(MAP_X/2)][y][(int)floor(MAP_Z/2)] = voxs[VOX_TEST];
                map.vox[(int)floor(MAP_X/2)][y][(int)floor(MAP_Z/2)+1] = voxs[VOX_TEST];
                map.vox[(int)floor(MAP_X/2)][y][(int)floor(MAP_Z/2)+2] = voxs[VOX_TEST];
                map.vox[(int)floor(MAP_X/2)][y][(int)floor(MAP_Z/2)+3] = voxs[VOX_TEST];

                map.vox[(int)floor(MAP_X/2)][y][(int)floor(MAP_Z/2)] = voxs[VOX_TEST];
                map.vox[(int)floor(MAP_X/2)+1][y][(int)floor(MAP_Z/2)] = voxs[VOX_TEST];
                map.vox[(int)floor(MAP_X/2)+2][y][(int)floor(MAP_Z/2)] = voxs[VOX_TEST];
                map.vox[(int)floor(MAP_X/2)+3][y][(int)floor(MAP_Z/2)] = voxs[VOX_TEST];
        }
        */


        Vec3 pos;
        Vec2 dir;
        pos.x = map.spawn_pos.x;
        pos.y = map.spawn_pos.y;
        pos.z = map.spawn_pos.z;
        dir.x = map.spawn_rot.x;
        dir.y = map.spawn_rot.y;

        Cam c = cam_init(pos, dir, RES_X, RES_Y, deg_to_rad(60));
        cam_calculate_vFOV(&c);

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
                        c.pos.z+=0.5;
                }
                if(input_key_pressed('a'))
                {
                        c.pos.x-=0.5;
                }
                if(input_key_pressed('s'))
                {
                        c.pos.z-=0.5;
                }
                if(input_key_pressed('d'))
                {
                        c.pos.x+=0.5;
                }

                if(input_key_pressed('i'))
                {
                        c.rot.x+= 0.1;
                }
                if(input_key_pressed('k'))
                {
                        c.rot.x-= 0.1;
                }
                if(input_key_pressed('l'))
                {
                        c.rot.y+= 0.1;
                }
                if(input_key_pressed('j'))
                {
                        c.rot.y-=0.1;
                }


                cam_rays_reset(&c);
                cam_calculate_ray_trajectories(&c);
                capture_and_pass(&c, &buffer, &map);
                buffer_translate_ray_data(&buffer);
                buffer_print(&buffer);
                

                frame_end = record_time();
                printf("| FPS: %.1f | Frametime: %.1fms | Min Frametime: %.1f", time_frame_time_to_fps(time_diff(frame_start, frame_end)), timestamp_to_ms(time_diff(frame_start, frame_end)), FRAMETIME_MIN);
                
                if (timestamp_to_ms(time_diff(frame_start, frame_end)) < FRAMETIME_MIN)
                {
                        sleep_ms(FRAMETIME_MIN - timestamp_to_ms(time_diff(frame_start, frame_end))); 
                }
        }

        input_restore();
        
        printf("\033[2J");

        cam_destroy(&c);
        buffer_destroy(&buffer);

        return 0;
}
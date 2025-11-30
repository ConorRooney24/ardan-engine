#include <stdio.h>

#include "include/map.h"
#include "include/material.h"
#include "include/config.h"
#include "include/renderer.h"
#include "include/camera.h"
#include "include/trig.h"

int main(void)
{
        config.max_ray_distance = 200;
        config.max_ray_step = 1;
        materials_init(materials);

        Buffer buffer = buffer_init(16*8, 9*8);
        Map map = map_init((Vec3i){100,100,100});

        for (int x = 0; x < map.size.x; x++)
        {
                for (int y = 0; y < map.size.y; y++)
                {
                        for (int z = 0; z < map.size.z; z++)
                        {
                                //map.grid[x][y][z].mat_id = MAT_AIR;
                                //map.grid[x][y][z].mat_id = MAT_BLOCK_GREEN;
                                if (z == map.size.z-1)
                                {
                                        map.grid[x][y][z].mat_id = MAT_BLOCK_GREEN;
                                }
                                
                        }
                }
        }

        Vec3 pos;
        Vec2 dir_deg;
        pos.x = 50;
        pos.y = 50;
        pos.z = 0;
        dir_deg.x = 0;
        dir_deg.y = 0;

        Camera c = camera_init(16*8, 9*8, 90, pos, dir_deg);
        camera_calculate_vFOV(&c);

        camera_calculate_ray_trajectories(&c);
        capture_and_pass(&c, &buffer, &map);

        buffer_translate_ray_data(&buffer);
        buffer_print(&buffer);

        camera_destroy(&c);
        buffer_destroy(&buffer);

        return 0;
}
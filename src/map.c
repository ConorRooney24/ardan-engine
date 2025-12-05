// map.c

#include <stdlib.h>
#include "../include/map.h"
#include "../include/material.h"

Map map_init(Vec3i size)
{
        Map map;
        map.size = size;

        map.vox = (Vox***)malloc(size.x * sizeof(Vox**));

        for (int x = 0; x < size.x; x++)
        {
                map.vox[x] = (Vox**)malloc(size.y * sizeof(Vox*));

                for (int y = 0; y < size.y; y++)
                {
                        map.vox[x][y] = (Vox *)malloc(size.z * sizeof(Vox));

                        for (int z = 0; z < size.z; z++)
                        {                                
                                map.vox[x][y][z] = voxs[VOX_UNDEFINED];
                        }
                }
        }
        return map;
}

void map_destroy(Map *map)
{
        for (int x = 0; x < map->size.x; x++)
        {
                for (int y = 0; y < map->size.y; y++)
                {
                        free(map->vox[x][y]);
                }

                free(map->vox[x]);
        }

        free(map->vox);

        map->vox = NULL;
        map->size = (Vec3i){0,0,0};
}
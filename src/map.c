// map.c

#include <stdlib.h>
#include "../include/map.h"
#include "../include/material.h"

Map map_init(Vec3i size)
{
        Map map;
        map.size = size;

        map.grid = (Material***)malloc(size.x * sizeof(Material**));

        for (int x = 0; x < size.x; x++)
        {
                map.grid[x] = (Material**)malloc(size.y * sizeof(Material*));

                for (int y = 0; y < size.y; y++)
                {
                        map.grid[x][y] = (Material *)malloc(size.z * sizeof(Material));

                        for (int z = 0; z < size.z; z++)
                        {
                                map.grid[x][y][z] = materials[MAT_UNDEFINED];
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
                        free(map->grid[x][y]);
                }

                free(map->grid[x]);
        }

        free(map->grid);

        map->grid = NULL;
        map->size = (Vec3i){0,0,0};
}
// map.c

#include "../include/map.h"
#include "../include/material.h"

Map map;

void map_init()
{
        for (int x = 0; x < MAP_SIZE_X; x++)
        {
                for (int y = 0; y < MAP_SIZE_Y; y++)
                {
                        for (int z = 0; z < MAP_SIZE_Z; z++)
                        {
                                map.grid[x][y][z] = MAT_AIR;
                        }
                }
        }
}
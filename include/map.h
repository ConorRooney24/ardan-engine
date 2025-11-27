// map.h
//
// Defines the struct for the game map and provides utilities for it.
// 
// The map itself is a 3D array, where each index represents a different material.

#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#define MAP_SIZE_X 100
#define MAP_SIZE_Y 100
#define MAP_SIZE_Z 100

typedef struct Map
{
        uint8_t grid[MAP_SIZE_X][MAP_SIZE_Y][MAP_SIZE_Z];
} Map;

void map_init(Map* map); // Sets every voxel on the map to the default material (MAT_AIR).

#endif // MAP_H
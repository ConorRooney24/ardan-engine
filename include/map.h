// map.h
//
// Defines the struct for the game map and provides utilities for it.
// 
// The map itself is a 3D array, where each index represents a different material.

#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include "vector.h"
#include "material.h"

typedef struct Map
{
        Vec3 size;
        Material ***grid; // TODO, Maybe make these "static objects" instead of materials
} Map;

Map map_init(Vec3 size);
void map_destroy(Map *map);

#endif // MAP_H
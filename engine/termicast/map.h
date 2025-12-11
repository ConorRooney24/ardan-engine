// map.h
//
// Defines the struct for the game map and provides utilities for it.
// 
// The map itself is a 3D array, where each index represents a different material.

#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include "../vector.h"
#include "material.h"
#include "text.h"
#include "vox.h"

typedef struct Map
{
        char *name;
        Vec3i size;
        Vec3 spawn_pos;
        Vec2 spawn_rot;
        Vox ***vox;
} Map;

Map map_init(Vec3i size);
void map_destroy(Map *map);

Map map_load_file(const char *file); // Inits a map from a file

#endif // MAP_H
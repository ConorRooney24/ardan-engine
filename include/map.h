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

void map_load_file(Map *map, const char *file); // Takes an initialised map and populates it with data from a map file.

#endif // MAP_H
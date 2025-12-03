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

// Used for describing the faces of a voxel
#define VOX_FACE_X_MINUS 0
#define VOX_FACE_X_PLUS 1
#define VOX_FACE_Y_MINUS 2
#define VOX_FACE_Y_PLUS 3
#define VOX_FACE_Z_MINUS 4
#define VOX_FACE_Z_PLUS 5

typedef struct Vox // Voxels
{
        Material mat;
        Text text;
} Vox;

typedef struct Map
{
        Vec3i size;
        Vox ***vox;
} Map;

Map map_init(Vec3i size);
void map_destroy(Map *map);

#endif // MAP_H
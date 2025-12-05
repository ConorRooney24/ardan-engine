// vox.h
//
// Voxels

#ifndef VOXEL_H
#define VOXEL_H

#include <stdint.h>
#include "material.h"
#include "text.h"

// Used for describing the faces of a voxel
#define VOX_FACE_X_MINUS 0
#define VOX_FACE_X_PLUS 1
#define VOX_FACE_Y_MINUS 2
#define VOX_FACE_Y_PLUS 3
#define VOX_FACE_Z_MINUS 4
#define VOX_FACE_Z_PLUS 5

#define VOX_UNDEFINED 0 
#define VOX_TEST 1
#define VOX_BRICK_WALL_RED 2
#define VOX_FLOOR_BOARDS_DIRTY_YELLOW 3
#define VOX_AIR 255

typedef struct Vox // Voxels
{
        uint8_t id;
        Material mat;
        Text text;
} Vox;

extern Vox voxs[256];

void map_voxs_init();

#endif // VOXEL_H
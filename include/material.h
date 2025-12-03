// material.h
//
// Contains information about the different materials and provides the material struct.

#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdint.h>
#include "col.h"

#define MAT_UNDEFINED 0
#define MAT_BLOCK_RED 1
#define MAT_BLOCK_GREEN 2
#define MAT_BLOCK_BLUE 3
#define MAT_AIR 255

typedef struct Material
{       
        uint8_t mat_id;
        float transparency;
        float reflectivity;
} Material;

extern Material materials[256]; // Array containing all materials allowing for mat information to be looked up easily.

void materials_init(Material* materials); // Inits the array
void material_print_info(Material material);

#endif // MATERIAL_H
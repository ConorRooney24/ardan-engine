// material.c

#include <stdio.h>
#include "../include/material.h"

Material materials[256];

void materials_init(Material* materials)
{
        for (int i = 0; i < 256; i++) // Set values for all undefined materials.
        {
                materials[i].mat_id = MAT_UNDEFINED;
                materials[i].col = (Col){255, 0, 255};
                materials[i].transparency = 0.7f;
                materials[i].reflectivity = 0.0f;
        }

        // Air
        materials[MAT_AIR].mat_id = MAT_AIR;
        materials[MAT_AIR].col = (Col){140, 194, 255};
        materials[MAT_AIR].transparency = 1.0f;
        materials[MAT_AIR].reflectivity = 0.0f;

        materials[MAT_BLOCK_RED].mat_id = MAT_BLOCK_RED;
        materials[MAT_BLOCK_RED].col = (Col){255, 0, 0};
        materials[MAT_BLOCK_RED].transparency = 0.0f;
        materials[MAT_BLOCK_RED].reflectivity = 0.0f;

        materials[MAT_BLOCK_GREEN].mat_id = MAT_BLOCK_GREEN;
        materials[MAT_BLOCK_GREEN].col = (Col){0, 255, 0};
        materials[MAT_BLOCK_GREEN].transparency = 0.0f;
        materials[MAT_BLOCK_GREEN].reflectivity = 0.0f;

        materials[MAT_BLOCK_BLUE].mat_id = MAT_BLOCK_BLUE;
        materials[MAT_BLOCK_BLUE].col = (Col){0, 0, 255};
        materials[MAT_BLOCK_BLUE].transparency = 0.0f;
        materials[MAT_BLOCK_BLUE].reflectivity = 0.0f;
}

void material_print_info(Material material)
{
        printf("Material ID: %d\nRGB: %d, %d, %d\nTransparency: %.2f\%\nReflectivity: %.2f\%\n", material.mat_id, material.col.r, material.col.g, material.col.b, material.transparency*100, material.reflectivity*100);
}
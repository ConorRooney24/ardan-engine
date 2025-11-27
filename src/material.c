// material.c

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
        materials[0].mat_id = MAT_AIR;
        materials[0].col = (Col){0, 0, 0};
        materials[0].transparency = 1.0f;
        materials[0].reflectivity = 0.0f;
}
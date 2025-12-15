// material.c

#include <stdio.h>
#include "material.h"

Material materials[256];

void materials_init(Material* materials)
{
        for (int i = 0; i < 256; i++) // Set values for all undefined materials.
        {
                materials[i].mat_id = MAT_UNDEFINED;
                materials[i].transparency = 0.7f;
                materials[i].reflectivity = 0.0f;
        }
        
        materials[MAT_BLOCK].mat_id = MAT_BLOCK;
        materials[MAT_BLOCK].transparency = 0.0f;
        materials[MAT_BLOCK].reflectivity = 0.0f;

        materials[MAT_AIR].mat_id = MAT_AIR;
        materials[MAT_AIR].transparency = 1.0f;
        materials[MAT_AIR].reflectivity = 0.0f;
}

void material_print_info(Material material)
{
        printf("Material ID: %d\nTransparency: %.2f\%\nReflectivity: %.2f\%\n", material.mat_id, material.transparency*100, material.reflectivity*100);
}
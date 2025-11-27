#include "include/map.h"
#include "include/material.h"

int main(void)
{
        Map map;
        map_init(&map);

        materials_init(materials);
        material_print_info(materials[0]);

        return 0;
}
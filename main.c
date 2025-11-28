#include "include/map.h"
#include "include/material.h"
#include "include/config.h"
#include "include/renderer.h"

int main(void)
{
        Map map;
        map_init(&map);

        materials_init(materials);
        material_print_info(materials[0]);

        config.display_height = DISPLAY_HEIGHT_DEFAULT;
        config.display_width = DISPLAY_WIDTH_DEFAULT;

        buffer_init();
        buffer_print();

        return 0;
}
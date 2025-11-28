#include "include/map.h"
#include "include/material.h"
#include "include/config.h"
#include "include/renderer.h"

int main(void)
{
        map_init();

        materials_init(materials);
        material_print_info(materials[0]);

        config.display_height = DISPLAY_HEIGHT_DEFAULT;
        config.display_width = DISPLAY_WIDTH_DEFAULT;

        Buffer buffer = buffer_init(config.display_width, config.display_height);
        buffer_print(&buffer);
        buffer_destroy(&buffer);


        return 0;
}
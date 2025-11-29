#include <stdio.h>

#include "include/map.h"
#include "include/material.h"
#include "include/config.h"
#include "include/renderer.h"
#include "include/camera.h"
#include "include/trig.h"

int main(void)
{
        map_init();

        materials_init(materials);
        material_print_info(materials[0]);

        config.display_height = DISPLAY_HEIGHT_DEFAULT;
        config.display_width = DISPLAY_WIDTH_DEFAULT;
        config.hFOV = 90;

        Buffer buffer = buffer_init(config.display_width, config.display_height);
        buffer_print(&buffer);
        buffer_destroy(&buffer);

        Vec3 a;
        a.x = 1;
        a.y = 2;
        a.z = 3;

        Camera c = camera_init(1920, 1080, config.hFOV, a, a);
        camera_calculate_vFOV(&c);
        camera_print_info(&c);
        printf("90 deg to rad: %f\n", deg_to_rad(90));
        printf("1 rad to deg: %f\n", rad_to_deg(1));
        camera_destroy(&c);

        return 0;
}
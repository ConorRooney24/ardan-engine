// vox.c

#include "vox.h"

Vox voxs[256];

void map_voxs_init()
{
        for (int i = 0; i < 256; i++)
        {
                voxs[i].id = VOX_UNDEFINED;
                voxs[i].mat = &materials[MAT_UNDEFINED];
                voxs[i].text = &texts[TEXT_UNDEFINED];
        }

        voxs[VOX_TEST].id = VOX_TEST;
        voxs[VOX_TEST].mat = &materials[MAT_BLOCK];
        voxs[VOX_TEST].text = &texts[TEXT_TEST];
        
        voxs[VOX_BRICK_WALL_RED].id = VOX_BRICK_WALL_RED;
        voxs[VOX_BRICK_WALL_RED].mat = &materials[MAT_BLOCK];
        voxs[VOX_BRICK_WALL_RED].text = &texts[TEXT_BRICK_WALL_RED];

        voxs[VOX_FLOOR_BOARDS_DIRTY_YELLOW].id = VOX_FLOOR_BOARDS_DIRTY_YELLOW;
        voxs[VOX_FLOOR_BOARDS_DIRTY_YELLOW].mat = &materials[MAT_BLOCK];
        voxs[VOX_FLOOR_BOARDS_DIRTY_YELLOW].text = &texts[TEXT_FLOOR_BOARDS_DIRTY_YELLOW];

        voxs[VOX_PAINT_BEIGE].id = VOX_PAINT_BEIGE;
        voxs[VOX_PAINT_BEIGE].mat = &materials[MAT_BLOCK];
        voxs[VOX_PAINT_BEIGE].text = &texts[TEXT_PAINT_BEIGE];

        voxs[VOX_AIR].id = VOX_AIR;
        voxs[VOX_AIR].mat = &materials[MAT_AIR];
        voxs[VOX_AIR].text = &texts[TEXT_UNDEFINED];
}
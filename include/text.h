// text.h
//
// Contains information regarding Textures

#ifndef TEXT_H
#define TEXT_H

#include "col.h"

#define TEXT_RES_X 16
#define TEXT_RES_Y 16

#define TEXT_UNDEFINED 0
#define TEXT_BRICK_WALL_RED 1
#define TEXT_FLOOR_BOARDS_DIRTY_YELLOW 2
#define TEXT_TEST 255

typedef struct Text
{
        uint8_t id;
        Col col_map[6][TEXT_RES_Y][TEXT_RES_X]; // 6 Sides of a cube
} Text;

extern Text texts[256];

void text_init(); // Initialises the texts array with textures
int text_load_from_img(const char *filename, uint8_t index);

#endif // TEXT_H
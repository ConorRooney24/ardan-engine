// text.h
//
// Contains information regarding Textures

#ifndef TEXT_H
#define TEXT_H

#include "col.h"

#define TEXT_SIZE_X 4
#define TEXT_SIZE_Y 4

#define TEXT_UNDEFINED 0

typedef struct Text
{
        uint8_t id;
        Col col_map[TEXT_SIZE_Y][TEXT_SIZE_X];
} Text;

extern Text texts[256];

void text_init(); // Initialises the texts array with textures

#endif // TEXT_H
// text.c

#include "../include/text.h"

Text texts[256];

void text_init()
{
        for (int i = 0; i < 256; i++)
        {
                texts[i].id = TEXT_UNDEFINED;

                for (int y = 0; y < 4; y++)
                {
                        for (int x = 0; x < 4; x++)
                        {
                                texts[i].col_map[y][x] = COL_UNDEFINED;
                        }
                }
        }
}
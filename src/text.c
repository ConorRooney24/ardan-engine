// text.c

#include "../include/text.h"
#include "../include/map.h"

Text texts[256];

void text_init()
{
        for (int i = 0; i < 256; i++)
        {
                texts[i].id = TEXT_UNDEFINED;
                
                for (int face = 0; face < 6; face++)
                {
                        for (int y = 0; y < TEXT_RES_Y; y++)
                        {
                                for (int x = 0; x < TEXT_RES_X; x++)
                                {
                                        if (face == VOX_FACE_X_MINUS || face == VOX_FACE_X_PLUS)
                                        {
                                                if ((y+x)%2==0)
                                                {
                                                        texts[i].col_map[face][y][x] = COL_GREEN;
                                                }
                                                else
                                                {
                                                        texts[i].col_map[face][y][x] = (Col){0,0,0};
                                                }
                                        }
                                        else if (face == VOX_FACE_Y_MINUS || face == VOX_FACE_Y_PLUS)
                                        {
                                                if ((y+x)%2==0)
                                                {
                                                        texts[i].col_map[face][y][x] = COL_RED;
                                                }
                                                else
                                                {
                                                        texts[i].col_map[face][y][x] = (Col){0,0,0};
                                                }
                                        }
                                        else if (face == VOX_FACE_Z_MINUS || face == VOX_FACE_Z_PLUS)
                                        {
                                                if ((y+x)%2==0)
                                                {
                                                        texts[i].col_map[face][y][x] = COL_BLUE;
                                                }
                                                else
                                                {
                                                        texts[i].col_map[face][y][x] = (Col){0,0,0};
                                                }
                                        }
                                }
                        }
                }
        }
}
// text.c

#include "../include/text.h"
#include "../include/map.h"
#include "../include/load_bar.h"

Text texts[256];

void text_init()
{
        int prog_counter = 1;

        for (int i = 0; i < 256; i++)
        {
                printf("Initialising Textures [%d / %d] - ", prog_counter, 256);
                load_bar_print((float)prog_counter, 256.0f, 20);
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
                                                        texts[i].col_map[face][y][x] = (Col){0,100,0};
                                                        //texts[i].col_map[face][y][x] = col_skew(COL_GREEN, 400);
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
                                                        texts[i].col_map[face][y][x] = (Col){100,0,0};
                                                        //texts[i].col_map[face][y][x] = col_skew(COL_RED, 400);
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
                                                        texts[i].col_map[face][y][x] = (Col){0,0,100};
                                                        //texts[i].col_map[face][y][x] = col_skew(COL_BLUE, 400);
                                                }
                                        }
                                        
                                }
                        }
                }
                prog_counter++;
        }

        printf("Textures initialised!\n");
}
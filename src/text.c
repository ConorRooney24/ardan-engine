// text.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <png.h>
#include "../include/text.h"
#include "../include/map.h"
#include "../include/load_bar.h"

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
        }

        texts[TEXT_TEST].id = TEXT_TEST;
        text_load_from_img("res/text/TEXT_TEST.png", TEXT_TEST);
}

int text_load_from_img(const char *filename, uint8_t index) // This function is completely AI generated. I dont understand PNG's.
{
        FILE *fp = NULL;
        png_structp png_ptr = NULL;
        png_infop info_ptr = NULL;
        png_bytep *row_pointers = NULL;
        int success = 0;

        if (!filename) return 0;

        if (index >= 256) 
        {
                fprintf(stderr, "text_load_from_img: index out of range (%u)\n", index);
                return 0;
        }

        fp = fopen(filename, "rb");

        if (!fp) 
        {
                fprintf(stderr, "text_load_from_img: cannot open '%s'\n", filename);
                return 0;
        }

        unsigned char header[8];
        if (fread(header, 1, 8, fp) != 8) 
        {
                fprintf(stderr, "text_load_from_img: failed to read header\n");
                goto cleanup;
        }
        if (png_sig_cmp(header, 0, 8)) 
        {
                fprintf(stderr, "text_load_from_img: '%s' is not a PNG\n", filename);
                goto cleanup;
        }

        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr) 
        {
                fprintf(stderr, "text_load_from_img: png_create_read_struct failed\n");
                goto cleanup;
        }

        info_ptr = png_create_info_struct(png_ptr);

        if (!info_ptr) 
        {
                fprintf(stderr, "text_load_from_img: png_create_info_struct failed\n");
                goto cleanup;
        }

        if (setjmp(png_jmpbuf(png_ptr))) 
        {
                fprintf(stderr, "text_load_from_img: libpng error while reading '%s'\n", filename);
                goto cleanup;
        }

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        png_uint_32 width  = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        png_byte color_type = png_get_color_type(png_ptr, info_ptr);
        png_byte bit_depth  = png_get_bit_depth(png_ptr, info_ptr);

        const png_uint_32 expected_w = (png_uint_32)(TEXT_RES_X * 2);
        const png_uint_32 expected_h = (png_uint_32)(TEXT_RES_Y * 3);

        if (width != expected_w || height != expected_h) 
        {
                fprintf(stderr, "text_load_from_img: unexpected image size %u x %u (expected %u x %u)\n", (unsigned)width, (unsigned)height, (unsigned)expected_w, (unsigned)expected_h);
                goto cleanup;
        }

        if (bit_depth == 16)
        {
                png_set_strip_16(png_ptr);
        }
        

        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
                png_set_palette_to_rgb(png_ptr);
        }

        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        {
                png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        {
                png_set_tRNS_to_alpha(png_ptr);
        }

        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        {
                png_set_gray_to_rgb(png_ptr);
        }
        
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

        png_read_update_info(png_ptr, info_ptr);

        int channels = png_get_channels(png_ptr, info_ptr);

        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);

        if (!row_pointers)
        {
                fprintf(stderr, "text_load_from_img: out of memory\n");
                goto cleanup;
        }

        for (png_uint_32 y = 0; y < height; ++y) 
        {
                png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
                row_pointers[y] = (png_bytep)malloc(rowbytes);

                if (!row_pointers[y])
                {
                        fprintf(stderr, "text_load_from_img: out of memory (row %u)\n", (unsigned)y);
                        for (png_uint_32 yy = 0; yy < y; ++yy)
                        {
                                free(row_pointers[yy]);
                        }
                        free(row_pointers);
                        row_pointers = NULL;
                        goto cleanup;
                }
        }

        png_read_image(png_ptr, row_pointers);

        const int face_map[3][2] = {{ VOX_FACE_X_MINUS, VOX_FACE_X_PLUS }, { VOX_FACE_Y_MINUS, VOX_FACE_Y_PLUS }, { VOX_FACE_Z_MINUS, VOX_FACE_Z_PLUS }};

        for (int seg_row = 0; seg_row < 3; ++seg_row) 
        {
                for (int seg_col = 0; seg_col < 2; ++seg_col) 
                {
                        int face = face_map[seg_row][seg_col];

                        for (int fy = 0; fy < TEXT_RES_Y; ++fy) 
                        {
                                png_uint_32 global_y = (png_uint_32)(seg_row * TEXT_RES_Y + fy);
                                png_bytep row = row_pointers[global_y];

                                for (int fx = 0; fx < TEXT_RES_X; ++fx) 
                                {
                                        png_uint_32 global_x = (png_uint_32)(seg_col * TEXT_RES_X + fx);
                                        png_bytep px = &row[global_x * channels];

                                        texts[index].col_map[face][fy][fx].r = px[0];
                                        texts[index].col_map[face][fy][fx].g = px[1];
                                        texts[index].col_map[face][fy][fx].b = px[2];
                                }
                        }
                }
        }

        texts[index].id = index;

        success = 1;

        cleanup:
        if (row_pointers) 
        {
                for (png_uint_32 y = 0; y < height; ++y) 
                {
                        free(row_pointers[y]);
                }
                free(row_pointers);
        }

        if (png_ptr && info_ptr)
        {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        }
        else if (png_ptr) 
        {
                png_destroy_read_struct(&png_ptr, NULL, NULL);
        }

        if (fp) 
        {
                fclose(fp);
        }

        return success;
}
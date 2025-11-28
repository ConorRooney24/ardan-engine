// renderer.h
//
// Takes information about a rays journey and translates it into a pixels colour before printing to the terminal.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <stdlib.h>
#include "colour.h"
#include "config.h"

#define PIXEL_COLOUR_R_DEFAULT 255
#define PIXEL_COLOUR_G_DEFAULT 255
#define PIXEL_COLOUR_B_DEFAULT 255
#define PIXEL_CHARACTER_DEFAULT ' '
typedef struct Pixel
{
        Colour colour;
        char character;
} Pixel;

// extern Pixel buffer[DISPLAY_HEIGHT_MAX][DISPLAY_WIDTH_MAX]; old, non dynamic approach

typedef struct Buffer
{
    int width, height;
    Pixel **pixels;
} Buffer;

Buffer buffer_init(int width, int height);
void buffer_destroy(Buffer* buffer);
void buffer_print(Buffer* buffer);

#endif // RENDERER_H
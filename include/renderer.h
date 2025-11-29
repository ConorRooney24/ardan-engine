// renderer.h
//
// Takes information about a rays journey and translates it into a pixels colour before printing to the terminal.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <stdlib.h>
#include "colour.h"
#include "config.h"
#include "caster.h"

#define PIXEL_COLOUR_R_DEFAULT 255
#define PIXEL_COLOUR_G_DEFAULT 255
#define PIXEL_COLOUR_B_DEFAULT 255
#define PIXEL_CHARACTER_DEFAULT ' '
typedef struct Pixel
{
    Colour colour;
    char character;
} Pixel;

typedef struct Buffer
{
    int resolution_y, resolution_x; // y, x format for printing
    Ray **rays; // Raw ray data to be processed
    Pixel **pixels; // Processed pixels with colour information
} Buffer;

Buffer buffer_init(int width, int height);
void buffer_destroy(Buffer* buffer);
void buffer_print(Buffer* buffer);

#endif // RENDERER_H
// renderer.h
//
// Takes information about a rays journey and translates it into a pixels colour before printing to the terminal.
//

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

extern Pixel buffer[DISPLAY_HEIGHT_MAX][DISPLAY_WIDTH_MAX]; // In y, x. not x, y -  TODO, maybe make it so multiple buffers can exist (top down view camera screen for example)

void buffer_init();
void buffer_print();
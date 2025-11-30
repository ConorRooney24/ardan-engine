// colour.h
//
// provides the struct for 24 bit colour

#ifndef COLOUR_H
#define COLOUR_H

#include <stdint.h>

#define COLOUR_RED (Colour){255,0,0}
#define COLOUR_GREEN (Colour){0,255,0}
#define COLOUR_BLUE (Colour){0,0,255}

typedef struct Colour
{
        uint8_t r, g, b;
} Colour;

typedef Colour Col;

// TODO, make a colour_set_print(Col c) function

#endif // COLOUR_H
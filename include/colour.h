// colour.h
//
// provides the struct for 24 bit colour

#ifndef COLOUR_H
#define COLOUR_H

#include <stdint.h>
#include <stdio.h>
#include "rand.h"

#define COLOUR_RED (Colour){255,0,0}
#define COLOUR_GREEN (Colour){0,255,0}
#define COLOUR_BLUE (Colour){0,0,255}
#define COLOUR_UNDEFINED (Colour){255,0,255}

typedef struct Colour
{
        uint8_t r, g, b;
} Colour;

typedef Colour Col;

void colour_print_set(Col col);
void colour_print_reset();
Col colour_skew(Col c, int max_amount); // Skews a colours r,g, and b values by a random amount
Col colour_blend(Col a, Col b, float ratio);

#endif // COLOUR_H
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

typedef struct Colour
{
        uint8_t r, g, b;
} Colour;

typedef Colour Col;

void colour_print_set(Col col);
void colour_print_reset();
Col colour_skew(Col c, int max_ammount); // Skews a colours r,g, and b values by a random ammount

#endif // COLOUR_H
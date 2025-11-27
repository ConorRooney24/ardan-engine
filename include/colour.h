// colour.h
//
// provides the struct for 24 bit colour

#ifndef COLOUR_H
#define COLOUR_H

#include <stdint.h>

typedef struct Colour
{
        uint8_t r, g, b;
} Colour;

typedef Colour Col;

#endif // COLOUR_H
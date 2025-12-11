// col.h
//
// provides the struct for 24 bit colour

#ifndef COL_H
#define COL_H

#include <stdint.h>
#include <stdio.h>
#include "util/rand.h"

#define COL_RED (Col){255,0,0}
#define COL_GREEN (Col){0,255,0}
#define COL_BLUE (Col){0,0,255}
#define COL_UNDEFINED (Col){255,0,255}

typedef struct Col
{
        uint8_t r, g, b;
} Col;

void col_print_set(Col col);
void col_print_reset();
Col col_skew(Col c, int max_amount); // Skews a colours r,g, and b values by a random amount
Col col_blend(Col a, Col b, float ratio);

#endif // COL_H
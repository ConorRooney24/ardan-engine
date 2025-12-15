// ray_translator.h
//
// Offers a ray buffer. Will translate the ray information in its ray buffer into colour information and store that colour information in the global pixel buffer.

#ifndef RAY_TRANSLATOR_H
#define RAY_TRANSLATOR_H

#include "caster.h"
#include "../../io/buffer.h"

typedef struct RayBuffer
{
        int res_x, res_y;
        Ray **rays;
} RayBuffer;

RayBuffer ray_buffer_init(int res_x, int res_y);
void ray_buffer_destroy(RayBuffer *buffer);

void ray_buffer_translate(RayBuffer *ray_buffer, Buffer *buffer); // Translates the ray data into colour info and stores in the buffer

#endif // RAY_TRANSLATOR_H
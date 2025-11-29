// caster.h

#ifndef CASTER_H
#define CASTER_H

#include <stdbool.h>
#include "material.h"
#include "vector.h"

typedef struct Ray
{
        Vec3 position, direction;
        Vec2 direction_rad, direction_deg;
        float distance_travelled;
        bool collided;
        Material collided_material;
} Ray;

#endif // CASTER_H
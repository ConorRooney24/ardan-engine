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

Ray ray_init(Vec3 position, Vec2 direction_deg);
void ray_cast(Ray *ray); // Casts a ray and updates that rays journey information.

#endif // CASTER_H
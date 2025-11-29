// caster.h

#ifndef CASTER_H
#define CASTER_H

#include <stdbool.h>
#include "material.h"
#include "vector.h"
#include "map.h"

typedef struct Ray
{
        Vec3 position, direction;
        Vec3i position_floored;
        Vec2 direction_rad, direction_deg;
        float distance_travelled;
        bool collided;
        Material collided_material;
} Ray;

Ray ray_init(Vec3 position, Vec2 direction_deg); // TODO update to cover new values
void ray_cast(Ray *ray, Map *map, int max_distance, float max_step_size); // Casts a ray through a map and updates that rays journey information.
// TODO add ray destroyer

#endif // CASTER_H
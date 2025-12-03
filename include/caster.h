// caster.h

#ifndef CASTER_H
#define CASTER_H

#include <stdbool.h>
#include "material.h"
#include "vector.h"
#include "map.h"

typedef struct Ray
{
        Vec3 pos, vel;
        Vec3i pos_f; // Position floored
        Vec2 rot; // Pitch and Yaw in Radian.
        float distance_traveled;
        bool collided;
        Vox collided_vox; // The voxel the ray collided with
        Vec3 collision_pos; // The position of the ray when it collided
        Vec3 pos_last_alive; // The last position the ray was in before colliding
} Ray;

Ray ray_init(Vec3 pos, Vec2 rot);
void ray_cast_simple(Ray *ray, Map *map, int max_distance, float step_scalar); // Casts a ray through a map and updates that rays journey information. simply uses the same step scalar over and over with no change.
void ray_cast_jump(Ray *ray, Map *map, int max_distance, float step_scalar_coarse, float step_scalar_fine); // Will start with a scalar of 1 and will work out how many times it can move with that scalar before a collision. at a collision, it will move back one, and then switch to a more accurate scalar and then cast as usual.

#endif // CASTER_H
// caster.c

#include "../include/caster.h"
#include "../include/trig.h"

Ray ray_init(Vec3 position, Vec2 direction_deg)
{
    Ray ray;

    ray.collided = false;
    ray.collided_material = materials[MAT_UNDEFINED];
    
    ray.position = position;

    ray.direction_deg = direction_deg;

    ray.direction_rad.x = deg_to_rad(ray.direction_deg.x);
    ray.direction_rad.y = deg_to_rad(ray.direction_deg.y);

    ray.direction = direction_rad_to_direction(ray.direction_rad);

    ray.distance_travelled = 0;

    return ray;
}
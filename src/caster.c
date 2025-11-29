// caster.c

#include <math.h>
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

void ray_cast(Ray *ray, Map *map, int max_distance, float max_step_size)
{
	ray->direction = vec3_normalize(ray->direction);

	ray->direction.x *= max_step_size;
	ray->direction.y *= max_step_size;
	ray->direction.z *= max_step_size;

	while (ray->distance_travelled <= max_distance || ray->collided == true)
	{
		ray->position_floored.x = (int)floor(ray->position.x);
		ray->position_floored.y = (int)floor(ray->position.y);
		ray->position_floored.z = (int)floor(ray->position.z);
		
		if ((ray->position_floored.x >= map->size.x || ray->position_floored.x < 0)||(ray->position_floored.y >= map->size.y || ray->position_floored.y < 0)||(ray->position_floored.z >= map->size.z || ray->position_floored.z < 0)) // if out of bounds
		{
			ray->collided = true;
			ray->collided_material = materials[MAT_AIR];
		}
		else if ((map->grid[ray->position_floored.x][ray->position_floored.y][ray->position_floored.z].mat_id != MAT_UNDEFINED) || (map->grid[ray->position_floored.x][ray->position_floored.y][ray->position_floored.z].mat_id != MAT_AIR)) // if we have hit an object
		{
			ray->collided = true;
			ray->collided_material = materials[map->grid[ray->position_floored.x][ray->position_floored.y]->mat_id];
		}
		else // We are not out of bounds, and we have not collided, so we move forward
		{
			ray->position.x += ray->direction.x;
			ray->position.y += ray->direction.y;
			ray->position.z += ray->direction.z;
		}
	}
}
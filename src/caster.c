// caster.c

#include <math.h>
#include <stdio.h>
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
	// This function is very slow and causes visual bugs. TODO, Fix.
	// each ray has to increment in small values, a massive ammount of times,
	// especially in large open spaces. Not only that, but it is not accurate
	// When we increment a ray, and we then check that we collided, we dont
	// know the exact point of collision. We floor the position of the ray
	// once we realise it collided, and this is (I think) what is causing
	// the weird jagged edges on walls that should be flat. Decreasing the
	// step size solves this, but also has a large cost on speed. 
	// There are better algorithms such as 3D DDA which should be implemented

	ray->direction = vec3_normalize(ray->direction);

	ray->direction.x *= max_step_size;
	ray->direction.y *= max_step_size;
	ray->direction.z *= max_step_size;

	while (ray->distance_travelled <= max_distance && ray->collided == false)
	{
		ray->position_floored.x = (int)floor(ray->position.x);
		ray->position_floored.y = (int)floor(ray->position.y);
		ray->position_floored.z = (int)floor(ray->position.z);
		
		if ((ray->position_floored.x >= map->size.x || ray->position_floored.x < 0) || (ray->position_floored.y >= map->size.y || ray->position_floored.y < 0)||(ray->position_floored.z >= map->size.z || ray->position_floored.z < 0)) // if out of bounds
		{
			ray->collided = false;
			return;
		}
		else if (!map->grid[ray->position_floored.x][ray->position_floored.y][ray->position_floored.z].mat_id == MAT_UNDEFINED) // if we have hit an undefined cell
		{
			colour_print_set(COLOUR_RED);
			printf("Error: Ray collided with undefined material;");
			colour_print_reset();
			printf("\n");
			return;
		}
		else if (map->grid[ray->position_floored.x][ray->position_floored.y][ray->position_floored.z].transparency == 1.0f) // If the block is fully transparant (air)
		{
			ray->position.x += ray->direction.x;
			ray->position.y += ray->direction.y;
			ray->position.z += ray->direction.z;
			ray->distance_travelled += vec3_to_distance(ray->direction);
		}
		else // If not out of bounds, and havent hit undefined, and the block isnt fully transparant, we must have hit an object
		{
			ray->collided = true;
			ray->collided_material = map->grid[ray->position_floored.x][ray->position_floored.y][ray->position_floored.z];
			return;
		}
	}
}
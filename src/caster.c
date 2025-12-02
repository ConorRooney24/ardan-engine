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

void ray_cast_simple(Ray *ray, Map *map, int max_distance, float step_scalar)
{
	ray->direction = vec3_normalize(ray->direction);

	ray->direction.x *= step_scalar;
	ray->direction.y *= step_scalar;
	ray->direction.z *= step_scalar;

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

void ray_cast_jump(Ray *ray, Map *map, int max_distance, float step_scalar_coarse, float step_scalar_fine) // Currently will crash if a ray is out of bounds
{
	ray->direction = vec3_normalize(ray->direction);
	
	int jumpDistanceScalar = 0;
	bool jumpDistanceScalarFound = false;
	Vec3 simulated_ray_pos = ray->position;
	Vec3i simulated_ray_pos_floored = vec3_floor(simulated_ray_pos);
	Vec3 simulated_ray_direction = vec3_scale(ray->direction, step_scalar_coarse);

	while (!jumpDistanceScalarFound)
	{
		simulated_ray_pos_floored = vec3_floor(simulated_ray_pos);
		
		if (map->grid[simulated_ray_pos_floored.x][simulated_ray_pos_floored.y][simulated_ray_pos_floored.z].transparency == 1.0f) // if air
		{
			simulated_ray_pos = vec3_add(simulated_ray_pos, simulated_ray_direction);
			jumpDistanceScalar++;
		}
		else
		{
			jumpDistanceScalarFound = true;
		}
	}

	ray->position = vec3_add(ray->position, vec3_scale(ray->direction, jumpDistanceScalar - 1));

	ray->direction = vec3_scale(ray->direction, step_scalar_fine);

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
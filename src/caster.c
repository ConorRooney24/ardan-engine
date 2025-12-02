// caster.c

#include <math.h>
#include <stdio.h>
#include "../include/caster.h"
#include "../include/trig.h"

Ray ray_init(Vec3 pos, Vec2 dir_deg)
{
	Ray ray;

	ray.collided = false;
	ray.collided_material = materials[MAT_UNDEFINED];

	ray.pos = pos;

	ray.dir_deg = dir_deg;

	ray.dir_rad.x = deg_to_rad(ray.dir_deg.x);
	ray.dir_rad.y = deg_to_rad(ray.dir_deg.y);

	ray.dir = dir_rad_to_dir(ray.dir_rad);

	ray.distance_travelled = 0;

	return ray;
}

void ray_cast_simple(Ray *ray, Map *map, int max_distance, float step_scalar)
{
	ray->dir = vec3_normalize(ray->dir);

	ray->dir.x *= step_scalar;
	ray->dir.y *= step_scalar;
	ray->dir.z *= step_scalar;

	while (ray->distance_travelled <= max_distance && ray->collided == false)
	{
		ray->pos_floored.x = (int)floor(ray->pos.x);
		ray->pos_floored.y = (int)floor(ray->pos.y);
		ray->pos_floored.z = (int)floor(ray->pos.z);
		
		if ((ray->pos_floored.x >= map->size.x || ray->pos_floored.x < 0) || (ray->pos_floored.y >= map->size.y || ray->pos_floored.y < 0)||(ray->pos_floored.z >= map->size.z || ray->pos_floored.z < 0)) // if out of bounds
		{
			ray->collided = false;
			return;
		}
		else if (!map->grid[ray->pos_floored.x][ray->pos_floored.y][ray->pos_floored.z].mat_id == MAT_UNDEFINED) // if we have hit an undefined cell
		{
			colour_print_set(COLOUR_RED);
			printf("Error: Ray collided with undefined material;");
			colour_print_reset();
			printf("\n");
			return;
		}
		else if (map->grid[ray->pos_floored.x][ray->pos_floored.y][ray->pos_floored.z].transparency == 1.0f) // If the block is fully transparant (air)
		{
			ray->pos.x += ray->dir.x;
			ray->pos.y += ray->dir.y;
			ray->pos.z += ray->dir.z;
			ray->distance_travelled += vec3_to_distance(ray->dir);
		}
		else // If not out of bounds, and havent hit undefined, and the block isnt fully transparant, we must have hit an object
		{
			ray->collided = true;
			ray->collided_material = map->grid[ray->pos_floored.x][ray->pos_floored.y][ray->pos_floored.z];
			return;
		}
	}
}

void ray_cast_jump(Ray *ray, Map *map, int max_distance, float step_scalar_coarse, float step_scalar_fine) // Currently will crash if a ray is out of bounds
{
	ray->dir = vec3_normalize(ray->dir);
	
	int jumpDistanceScalar = 0;
	bool jumpDistanceScalarFound = false;
	Vec3 simulated_ray_pos = ray->pos;
	Vec3i simulated_ray_pos_floored = vec3_floor(simulated_ray_pos);
	Vec3 simulated_ray_dir = vec3_scale(ray->dir, step_scalar_coarse);

	while (!jumpDistanceScalarFound)
	{
		simulated_ray_pos_floored = vec3_floor(simulated_ray_pos);
		
		if (map->grid[simulated_ray_pos_floored.x][simulated_ray_pos_floored.y][simulated_ray_pos_floored.z].transparency == 1.0f) // if air
		{
			simulated_ray_pos = vec3_add(simulated_ray_pos, simulated_ray_dir);
			jumpDistanceScalar++;
		}
		else
		{
			jumpDistanceScalarFound = true;
		}
	}

	ray->pos = vec3_add(ray->pos, vec3_scale(ray->dir, jumpDistanceScalar - 1));

	ray->dir = vec3_scale(ray->dir, step_scalar_fine);

	while (ray->distance_travelled <= max_distance && ray->collided == false)
	{
		ray->pos_floored.x = (int)floor(ray->pos.x);
		ray->pos_floored.y = (int)floor(ray->pos.y);
		ray->pos_floored.z = (int)floor(ray->pos.z);
		
		if ((ray->pos_floored.x >= map->size.x || ray->pos_floored.x < 0) || (ray->pos_floored.y >= map->size.y || ray->pos_floored.y < 0)||(ray->pos_floored.z >= map->size.z || ray->pos_floored.z < 0)) // if out of bounds
		{
			ray->collided = false;
			return;
		}
		else if (!map->grid[ray->pos_floored.x][ray->pos_floored.y][ray->pos_floored.z].mat_id == MAT_UNDEFINED) // if we have hit an undefined cell
		{
			colour_print_set(COLOUR_RED);
			printf("Error: Ray collided with undefined material;");
			colour_print_reset();
			printf("\n");
			return;
		}
		else if (map->grid[ray->pos_floored.x][ray->pos_floored.y][ray->pos_floored.z].transparency == 1.0f) // If the block is fully transparant (air)
		{
			ray->pos.x += ray->dir.x;
			ray->pos.y += ray->dir.y;
			ray->pos.z += ray->dir.z;
			ray->distance_travelled += vec3_to_distance(ray->dir);
		}
		else // If not out of bounds, and havent hit undefined, and the block isnt fully transparant, we must have hit an object
		{
			ray->collided = true;
			ray->collided_material = map->grid[ray->pos_floored.x][ray->pos_floored.y][ray->pos_floored.z];
			return;
		}
	}
}
// caster.c

#include <math.h>
#include <stdio.h>
#include "../include/caster.h"
#include "../include/trig.h"

Ray ray_init(Vec3 pos, Vec2 rot)
{
	Ray ray;

	ray.collided = false;
	ray.collided_vox.mat = materials[MAT_UNDEFINED];
	ray.collided_vox.text = texts[TEXT_UNDEFINED];

	ray.distance_traveled = 0;

	ray.pos = pos;
	ray.pos_f = vec3_floor(pos);

	ray.rot = rot;

	ray.vel = rot_to_vel(rot);

	ray.collision_pos = pos;
	ray.pos_last_alive = pos;

	return ray;
}

void ray_cast_simple(Ray *ray, Map *map, int max_distance, float step_scalar)
{
	ray->vel = vec3_normalize(ray->vel);

	ray->vel.x *= step_scalar;
	ray->vel.y *= step_scalar;
	ray->vel.z *= step_scalar;

	while (ray->distance_traveled <= max_distance && ray->collided == false)
	{
		ray->pos_f.x = (int)floor(ray->pos.x);
		ray->pos_f.y = (int)floor(ray->pos.y);
		ray->pos_f.z = (int)floor(ray->pos.z);
		
		if ((ray->pos_f.x >= map->size.x || ray->pos_f.x < 0) || (ray->pos_f.y >= map->size.y || ray->pos_f.y < 0)||(ray->pos_f.z >= map->size.z || ray->pos_f.z < 0)) // if out of bounds
		{
			ray->collided = false;
			return;
		}
		else if (map->vox[ray->pos_f.x][ray->pos_f.y][ray->pos_f.z].mat.mat_id == MAT_UNDEFINED) // if we have hit an undefined cell
		{
			col_print_set(COL_RED);
			printf("Error: Ray collided with undefined material;");
			col_print_reset();
			printf("\n");
			return;
		}
		else if (map->vox[ray->pos_f.x][ray->pos_f.y][ray->pos_f.z].mat.transparency == 1.0f) // If the block is fully transparent (air)
		{
			ray->pos_last_alive = ray->pos; // Set last alive pos before moving

			ray->pos.x += ray->vel.x;
			ray->pos.y += ray->vel.y;
			ray->pos.z += ray->vel.z;
			ray->distance_traveled += vec3_to_distance(ray->vel);
		}
		else // If not out of bounds, and haven't hit undefined, and the block isn't fully transparent, we must have hit an object
		{
			ray->collided = true;
			ray->collided_vox = map->vox[ray->pos_f.x][ray->pos_f.y][ray->pos_f.z];
			ray->collision_pos = ray->pos;
			return;
		}
	}
}

void ray_cast_jump(Ray *ray, Map *map, int max_distance, float step_scalar_coarse, float step_scalar_fine) // Currently will crash if a ray is out of bounds
{
	ray->vel = vec3_normalize(ray->vel);
	
	int jumpDistanceScalar = 0;
	bool jumpDistanceScalarFound = false;
	Vec3 simulated_ray_pos = ray->pos;
	Vec3i simulated_ray_pos_floored = vec3_floor(simulated_ray_pos);
	Vec3 simulated_ray_vel = vec3_scale(ray->vel, step_scalar_coarse);

	while (!jumpDistanceScalarFound)
	{
		simulated_ray_pos_floored = vec3_floor(simulated_ray_pos);
		
		if (map->vox[simulated_ray_pos_floored.x][simulated_ray_pos_floored.y][simulated_ray_pos_floored.z].mat.transparency == 1.0f) // if air
		{
			simulated_ray_pos = vec3_add(simulated_ray_pos, simulated_ray_vel);
			jumpDistanceScalar++;
		}
		else
		{
			jumpDistanceScalarFound = true;
		}
	}

	ray->pos = vec3_add(ray->pos, vec3_scale(ray->vel, jumpDistanceScalar - 1));

	ray->vel = vec3_scale(ray->vel, step_scalar_fine);

	while (ray->distance_traveled <= max_distance && ray->collided == false)
	{
		ray->pos_f.x = (int)floor(ray->pos.x);
		ray->pos_f.y = (int)floor(ray->pos.y);
		ray->pos_f.z = (int)floor(ray->pos.z);

		if ((ray->pos_f.x >= map->size.x || ray->pos_f.x < 0) || (ray->pos_f.y >= map->size.y || ray->pos_f.y < 0)||(ray->pos_f.z >= map->size.z || ray->pos_f.z < 0)) // if out of bounds
		{
			ray->collided = false;
			return;
		}
		else if (map->vox[ray->pos_f.x][ray->pos_f.y][ray->pos_f.z].mat.mat_id == MAT_UNDEFINED) // if we have hit an undefined cell
		{
			col_print_set(COL_RED);
			printf("Error: Ray collided with undefined material;");
			col_print_reset();
			printf("\n");
			return;
		}
		else if (map->vox[ray->pos_f.x][ray->pos_f.y][ray->pos_f.z].mat.transparency == 1.0f) // If the block is fully transparent (air)
		{
			ray->pos_last_alive = ray->pos; // Set last alive pos before moving

			ray->pos.x += ray->vel.x;
			ray->pos.y += ray->vel.y;
			ray->pos.z += ray->vel.z;
			ray->distance_traveled += vec3_to_distance(ray->vel);
		}
		else // If not out of bounds, and haven't hit undefined, and the block isn't fully transparent, we must have hit an object
		{
			ray->collided = true;
			ray->collided_vox = map->vox[ray->pos_f.x][ray->pos_f.y][ray->pos_f.z];
			ray->collision_pos = ray->pos;
			return;
		}
	}
}
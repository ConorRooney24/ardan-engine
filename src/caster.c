// caster.c

#include <math.h>
#include <stdio.h>
#include <float.h>
#include "../include/caster.h"
#include "../include/trig.h"

Ray ray_init(Vec3 pos, Vec2 rot)
{
	Ray ray;

	ray.collided = false;
	ray.collided_vox.mat = &materials[MAT_UNDEFINED];
	ray.collided_vox.text = &texts[TEXT_UNDEFINED];

	ray.distance_traveled = 0;

	ray.pos = pos;
	ray.pos_f = vec3_floor(pos);

	ray.rot = rot;

	ray.vel = rot_to_vel(rot);

	ray.collision_pos = pos;
	ray.pos_last_alive = pos;

	ray.face_hit = 0;
	ray.tex_u = 0.0f;
	ray.tex_v = 0.0f;

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
		else if (map->vox[ray->pos_f.x][ray->pos_f.y][ray->pos_f.z].mat->mat_id == MAT_UNDEFINED) // if we have hit an undefined cell
		{
			col_print_set(COL_RED);
			printf("Error: Ray collided with undefined material;");
			col_print_reset();
			printf("\n");
			return;
		}
		else if (map->vox[ray->pos_f.x][ray->pos_f.y][ray->pos_f.z].mat->transparency == 1.0f) // If the block is fully transparent (air)
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
		
		if (map->vox[simulated_ray_pos_floored.x][simulated_ray_pos_floored.y][simulated_ray_pos_floored.z].mat->transparency == 1.0f) // if air
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
		else if (map->vox[ray->pos_f.x][ray->pos_f.y][ray->pos_f.z].mat->mat_id == MAT_UNDEFINED) // if we have hit an undefined cell
		{
			col_print_set(COL_RED);
			printf("Error: Ray collided with undefined material;");
			col_print_reset();
			printf("\n");
			return;
		}
		else if (map->vox[ray->pos_f.x][ray->pos_f.y][ray->pos_f.z].mat->transparency == 1.0f) // If the block is fully transparent (air)
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

void ray_cast_dda(Ray *ray, Map *map, int max_distance)
{
	const float EPSILON = 0.00001; // The epsilon we use to put difference between the last alive pos and the collided pos. Essentially has the same effect as the step scalar in the old algorithms except with no performance impact

	ray->vel = vec3_normalize(ray->vel);

	Vec3 dir = ray->vel;
	Vec3 origin_pos = ray->pos;

	Vec3i current_vox = vec3_floor(origin_pos);
	ray->pos_f.x = current_vox.x;
	ray->pos_f.y = current_vox.y;
	ray->pos_f.z = current_vox.z;

	if ((current_vox.x >= map->size.x || current_vox.x < 0) || (current_vox.y >= map->size.y || current_vox.y < 0) || (current_vox.z >= map->size.z || current_vox.z < 0))
	{
		ray->collided = false;
		return;
	}

	if (map->vox[current_vox.x][current_vox.y][current_vox.z].mat->mat_id == MAT_UNDEFINED)
	{
		col_print_set(COL_RED);
		printf("Error: Ray collided with undefined material;");
		col_print_reset();
		printf("\n");
		return;
	}

	if (map->vox[current_vox.x][current_vox.y][current_vox.z].mat->transparency != 1.0f)
	{
		ray->collided = true;
		ray->collided_vox = map->vox[current_vox.x][current_vox.y][current_vox.z];
		ray->collision_pos = origin_pos;
		return;
	}

	int8_t step_sign_x, step_sign_y, step_sign_z; // -1: back, 0: still, +1: forward
	step_sign_x = 0;
	step_sign_y = 0;
	step_sign_z = 0;
	if (dir.x > 0.0f) 
	{
		step_sign_x = 1;
	}
	else if (dir.x < 0.0f)
	{
		step_sign_x = -1;
	}

	if (dir.y > 0.0f) 
	{
		step_sign_y = 1;
	}
	else if (dir.y < 0.0f)
	{
		step_sign_y = -1;
	}

	if (dir.z > 0.0f) 
	{
		step_sign_z = 1;
	}
	else if (dir.z < 0.0f)
	{
		step_sign_z = -1;
	}


	float dist_step_x, dist_step_y, dist_step_z; // What constitutes moving one voxel in each axis
	if (dir.x != 0.0f)
	{
		dist_step_x = fabsf(1.0f / dir.x);
	}
	else
	{
		dist_step_x = FLT_MAX;
	}

	if (dir.y != 0.0f)
	{
		dist_step_y = fabsf(1.0f / dir.y);
	}
	else
	{
		dist_step_y = FLT_MAX;
	}

	if (dir.z != 0.0f)
	{
		dist_step_z = fabsf(1.0f / dir.z);
	}
	else
	{
		dist_step_z = FLT_MAX;
	}

    
	float dist_to_boundary_x, dist_to_boundary_y, dist_to_boundary_z; // dist from origin to next boundary along the ray in each axis
	dist_to_boundary_x = FLT_MAX; 
	dist_to_boundary_y = FLT_MAX;
	dist_to_boundary_z = FLT_MAX;

	if (step_sign_x > 0)
	{
		dist_to_boundary_x = ((float)(current_vox.x + 1) - origin_pos.x) / dir.x;
	}
	else if (step_sign_x < 0)
	{
		dist_to_boundary_x = (origin_pos.x - (float)current_vox.x) / (-dir.x);
	}
		
	if (step_sign_y > 0)
	{
		dist_to_boundary_y = ((float)(current_vox.y + 1) - origin_pos.y) / dir.y;
	}
	else if (step_sign_y < 0)
	{
		dist_to_boundary_y = (origin_pos.y - (float)current_vox.y) / (-dir.y);
	}

	if (step_sign_z > 0)
	{
		dist_to_boundary_z = ((float)(current_vox.z + 1) - origin_pos.z) / dir.z;
	}
	else if (step_sign_z < 0)
	{
		dist_to_boundary_z = (origin_pos.z - (float)current_vox.z) / (-dir.z);
	}
        
	float dist_along_ray_current = 0.0f; // the parametric distance along the ray we have traveled so far

	while (dist_along_ray_current <= (float)max_distance && ray->collided == false)
	{
		int axis_chosen = 0; // x: 0, y: 1, z: 2

		float dist_next = dist_to_boundary_x;
		if (dist_to_boundary_y < dist_next)
		{
			dist_next = dist_to_boundary_y;
			axis_chosen = 1;
		}
		if (dist_to_boundary_z < dist_next)
		{
			dist_next = dist_to_boundary_z;
			axis_chosen = 2;
		}

		ray->pos_last_alive = vec3_add(origin_pos, vec3_scale(dir, dist_next - EPSILON));

		ray->pos = vec3_add(origin_pos, vec3_scale(dir, dist_next));

		ray->distance_traveled += (dist_next - dist_along_ray_current);
		dist_along_ray_current = dist_next;

		// If we hit an exact corner we do the following
		const float boundary_eps = 0.00001f;

		bool stepped_x, stepped_y, stepped_z;
		stepped_x = false;
		stepped_y = false;
		stepped_z = false;

		if (fabsf(dist_to_boundary_x - dist_next) <= boundary_eps)
		{
			current_vox.x += step_sign_x;
			dist_to_boundary_x += dist_step_x;
			stepped_x = true;
		}

		if (fabsf(dist_to_boundary_y - dist_next) <= boundary_eps)
		{
			current_vox.y += step_sign_y;
			dist_to_boundary_y += dist_step_y;
			stepped_y = true;
		}

		if (fabsf(dist_to_boundary_z - dist_next) <= boundary_eps)
		{
			current_vox.z += step_sign_z;
			dist_to_boundary_z += dist_step_z;
			stepped_z = true;
		}

		// Face detection and UV calculation

		int face_hit_local = -1;
		float fx = ray->pos.x - floorf(ray->pos.x);
		float fy = ray->pos.y - floorf(ray->pos.y);
		float fz = ray->pos.z - floorf(ray->pos.z);

		if (fx < 0.0f)
		{
			fx += 1.0f;
		}
		if (fy < 0.0f)
		{
			fy += 1.0f;
		}
		if (fz < 0.0f)
		{
			fz += 1.0f;
		}

		// If more than one axis stepped:
		int axes = 0;
		if (stepped_x)
		{
			axes++;
		}
		if (stepped_y)
		{
			axes++;
		}
		if (stepped_z)
		{
			axes++;
		}

		if (axes > 1)
		{
			float adx, ady, adz; 
			adx = fabsf(dir.x);
			ady = fabsf(dir.y);
			adz = fabsf(dir.z);

			if (adx >= ady && adx >= adz)
			{
				stepped_y = false; 
				stepped_z = false;
			}
			else if (ady >= adx && ady >= adz)
			{
				stepped_x = false; 
				stepped_z = false; 
			}
			else
			{
				stepped_x = false; 
				stepped_y = false;
			}
		}

		//  Now we pick a face
		if (stepped_x)
		{
			if (step_sign_x > 0)
			{
				face_hit_local = VOX_FACE_X_MINUS; 
			}
			else
			{
				face_hit_local = VOX_FACE_X_PLUS;
			}

			ray->tex_u = fz;
			ray->tex_v = fy;

			if (step_sign_x < 0) // Flip text
			{
				ray->tex_u = 1.0f - ray->tex_u;
			}
		}
		else if (stepped_y)
		{
			if (step_sign_y > 0)
			{
				face_hit_local = VOX_FACE_Y_MINUS;
			}
			else
			{
				face_hit_local = VOX_FACE_Y_PLUS;
			}

			ray->tex_u = fx;
			ray->tex_v = fz;

			if (step_sign_y < 0) // Flip
			{
				ray->tex_v = 1.0f - ray->tex_v;
			}
		}
		else if (stepped_z)
		{
			if (step_sign_z > 0)
			{
				face_hit_local = VOX_FACE_Z_MINUS;
			}
			else
			{
				face_hit_local = VOX_FACE_Z_PLUS;
			}

			ray->tex_u = fx;
			ray->tex_v = fy;

			if (step_sign_z < 0) // flip
			{
				ray->tex_u = 1.0f - ray->tex_u;
			}
		}
		else // If we didn't step in any axis we go back to the dominant axis
		{
			float dx, dy, dz;
			dx = fabsf(dir.x);
			dy = fabsf(dir.y);
			dz = fabsf(dir.z);

			if (dx >= dy && dx >= dz) 
			{
				if (dir.x > 0)
				{
					face_hit_local = VOX_FACE_X_MINUS;
				}
				else
				{
					face_hit_local = VOX_FACE_X_PLUS;
				}
				
				ray->tex_u = fz; 
				ray->tex_v = fy; 
			}
			else if (dy >= dx && dy >= dz)
			{
				if (dir.y > 0)
				{
					face_hit_local = VOX_FACE_Y_MINUS;
				}
				else
				{
					face_hit_local = VOX_FACE_Y_PLUS;
				}
				
				ray->tex_u = fx;
				ray->tex_v = fz; 
			}
			else
			{
				if (dir.z > 0)
				{
					face_hit_local = VOX_FACE_Z_MINUS;
				}
				else
				{
					face_hit_local = VOX_FACE_Z_PLUS;
				}

				ray->tex_u = fx; 
				ray->tex_v = fy; 
			}
		}

		// clamp UV's
		if (ray->tex_u < 0.0f)
		{
			ray->tex_u = 0.0f;
		}
		else if (ray->tex_u > 1.0f) 
		{
			ray->tex_u = 1.0f;
		}

		if (ray->tex_v < 0.0f)
		{
			ray->tex_v = 0.0f;
		}
		else if (ray->tex_v > 1.0f)
		{
			ray->tex_v = 1.0f;
		}

		ray->face_hit = face_hit_local;

		ray->pos_f.x = current_vox.x;
		ray->pos_f.y = current_vox.y;
		ray->pos_f.z = current_vox.z;

		if ((current_vox.x >= map->size.x || current_vox.x < 0) || (current_vox.y >= map->size.y || current_vox.y < 0) || (current_vox.z >= map->size.z || current_vox.z < 0)) // Out of bounds
		{
			ray->collided = false;
			return;
		}

		if (map->vox[current_vox.x][current_vox.y][current_vox.z].mat->mat_id == MAT_UNDEFINED)
		{
			col_print_set(COL_RED);
			printf("Error: Ray collided with undefined material;");
			col_print_reset();
			printf("\n");
			return;
		}

		if (map->vox[current_vox.x][current_vox.y][current_vox.z].mat->transparency == 1.0f)
		{
			continue;
		}
		else
		{
			ray->collided = true;
			ray->collided_vox = map->vox[current_vox.x][current_vox.y][current_vox.z];
			ray->collision_pos = ray->pos;
			return;
		}
	}

	ray->collided = false;
	return;
}

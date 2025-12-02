// cam.c

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/trig.h"
#include "../include/cam.h"

Cam cam_init(int resolution_x, int resolution_y, float hFOV_deg, Vec3 pos, Vec2 dir_deg)
{
        Cam cam;

        cam.resolution_x = resolution_x;
        cam.resolution_y = resolution_y;

        cam.hFOV_deg = hFOV_deg;
        cam.hFOV = deg_to_rad(hFOV_deg);

        cam.pos = pos;

        cam.dir_deg = dir_deg;

        cam.dir_rad.x = deg_to_rad(cam.dir_deg.x);
        cam.dir_rad.y = deg_to_rad(cam.dir_deg.y);

        cam.dir = dir_rad_to_dir(cam.dir_rad);
        cam.dir = vec3_normalize(cam.dir);

        cam.rays = (Ray**)malloc(resolution_y * sizeof(Ray*));
        
        for (int i = 0; i < resolution_y; i++)
        {
                cam.rays[i] = malloc(resolution_x * sizeof(Ray));
        }

        for (int y = 0; y < resolution_y; y++)
        {
                for (int x = 0; x < resolution_x; x++)
                {
                        cam.rays[y][x] = ray_init(cam.pos, cam.dir_deg);
                }       
        }

        return cam;
}
void cam_destroy(Cam *cam)
{
        for (int y = 0; y < cam->resolution_y; y++)
        {
                free(cam->rays[y]);
        }

        free(cam->rays);

        cam->rays = NULL;
        cam->resolution_x = 0;
        cam->resolution_y = 0;
        cam->pos.x = 0;
        cam->pos.y = 0;
        cam->pos.z = 0;
        cam->dir.x = 0;
        cam->dir.y = 0;
        cam->dir.z = 0;
}

void cam_calculate_vFOV(Cam *cam)
{
        float aspect_ratio = (float)cam->resolution_y / (float)cam->resolution_x;
        cam->vFOV = 2.0f * atan(tan(cam->hFOV / 2.0f) * aspect_ratio);
        cam->vFOV_deg = rad_to_deg(cam->vFOV);
}

void cam_calculate_ray_trajectories(Cam *cam)
{
        double horizontal_ray_step = cam->hFOV / (cam->resolution_x - 1); // The distance between rays on the x axis in radian.
        double vertical_ray_step = cam->vFOV / (cam->resolution_y - 1); // The distance between rays on the y axis in radian.

        cam->dir_rad.x = rad_normalise(cam->dir_rad.x);
        cam->dir_rad.y = rad_normalise(cam->dir_rad.y);

        double horizontal_ray_start_pos = cam->dir_rad.y - (cam->hFOV / 2); // the yaw measure in radian which rays on the x axis will start casting from, and then move right by the x distance each step.
        //double vertical_ray_start_pos = cam->dir_rad.x - (cam->vFOV / 2);
        double vertical_ray_start_pos = cam->dir_rad.x + (cam->vFOV / 2.0);

        for (int y = 0; y < cam->resolution_y; y++) // for each ray
        {
                for (int x = 0; x < cam->resolution_x; x++)
                {
                        //double ray_pitch = vertical_ray_start_pos + (cam->resolution_y - y) * vertical_ray_step; // calculate the pitch and yaw of the given ray
                        double ray_pitch = vertical_ray_start_pos - y * vertical_ray_step;
                        double ray_yaw = horizontal_ray_start_pos + x * horizontal_ray_step;

                        cam->rays[y][x].dir_rad.x = rad_normalise(ray_pitch);
                        cam->rays[y][x].dir_rad.y = rad_normalise(ray_yaw);

                        cam->rays[y][x].dir_deg.x = rad_to_deg(cam->rays[y][x].dir_rad.x);
                        cam->rays[y][x].dir_deg.y = rad_to_deg(cam->rays[y][x].dir_rad.y);

                        cam->rays[y][x].dir = dir_rad_to_dir(cam->rays[y][x].dir_rad);
                } 
        }  
}

void capture_and_pass(Cam *cam, Buffer *buffer, Map *map)
{
        if (cam->resolution_x != buffer->resolution_x || cam->resolution_y != buffer->resolution_y)
        {
                colour_print_set(COLOUR_RED);
                printf("Error: Cam resolution does not match buffer resolution");
                colour_print_reset();
                printf("\n");
                return;
        }

        for (int y = 0; y < cam->resolution_y; y++)
        {
                for (int x = 0; x < cam->resolution_x; x++)
                {
                        //ray_cast_simple(&cam->rays[y][x], map, config.max_ray_distance, config.ray_step_scalar_fine);
                        ray_cast_jump(&cam->rays[y][x], map, config.max_ray_distance, config.ray_step_scalar_coarse, config.ray_step_scalar_fine);
                        buffer->rays[y][x] = cam->rays[y][x];
                }
        }
}

void cam_rays_reset(Cam *cam)
{
        for (int y = 0; y < cam->resolution_y; y++)
        {
                for (int x = 0; x < cam->resolution_x; x++)
                {
                        cam->rays[y][x].collided = false;
                        cam->rays[y][x].collided_material = materials[MAT_UNDEFINED];
                        cam->rays[y][x].dir = cam->dir;
                        cam->rays[y][x].dir_deg = cam->dir_deg;
                        cam->rays[y][x].dir_rad = cam->dir_rad;
                        cam->rays[y][x].distance_travelled = 0;
                        cam->rays[y][x].pos = cam->pos;
                }
        }
}

void cam_print_info(Cam *cam)
{
        printf("+--- Cam Info ---------------------------------------\n");
        printf("| pos (xyz):\tx = %.2f,\ty = %.2f,\tz = %.2f\n", cam->pos.x, cam->pos.y, cam->pos.z);
        printf("| dir (xyz):\tx = %.2f,\ty = %.2f,\tz = %.2f\n", cam->dir.x, cam->dir.y, cam->dir.z);
        printf("| dir (Deg):\tPitch = %.2f,\tYaw = %.2f\n", cam->dir_deg.x, cam->dir_deg.y);
        printf("| dir (Rad):\tPitch = %.2f,\tYaw = %.2f\n", cam->dir_rad.x, cam->dir_rad.y);
        printf("| FOV (Deg):\t\th = %.2f,\tv = %.2f\n", cam->hFOV_deg, cam->vFOV_deg);
        printf("| Resolution X:\t%d\tResolution Y:\t%d\n", cam->resolution_x, cam->resolution_y);
        printf("+-------------------------------------------------------\n");
}

void cam_print_ray_trajectories_deg(Cam *cam)
{
        printf("--- Ray trajectory matrix in deg ---\n");
        for (int y = 0; y < cam->resolution_y; y++)
        {
                for (int x = 0; x < cam->resolution_x; x++)
                {
                        printf("y%d, x%d [%.1f, %.1f], ", y, x, cam->rays[y][x].dir_deg.x, cam->rays[y][x].dir_deg.y);
                }
                printf("\n");
        }  
}

void cam_print_ray_trajectories_rad(Cam *cam)
{
        printf("--- Ray trajectory matrix in rad ---\n");
        for (int y = 0; y < cam->resolution_y; y++)
        {
                for (int x = 0; x < cam->resolution_x; x++)
                {
                        printf("[%.1f, %.1f]", cam->rays[y][x].dir_rad.x, cam->rays[y][x].dir_rad.y);
                }
                printf("\n");
        }  
}
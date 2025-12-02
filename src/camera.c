// camera.c

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/trig.h"
#include "../include/camera.h"

Camera camera_init(int resolution_x, int resolution_y, float hFOV_deg, Vec3 pos, Vec2 dir_deg)
{
        Camera camera;

        camera.resolution_x = resolution_x;
        camera.resolution_y = resolution_y;

        camera.hFOV_deg = hFOV_deg;
        camera.hFOV = deg_to_rad(hFOV_deg);

        camera.pos = pos;

        camera.dir_deg = dir_deg;

        camera.dir_rad.x = deg_to_rad(camera.dir_deg.x);
        camera.dir_rad.y = deg_to_rad(camera.dir_deg.y);

        camera.dir = dir_rad_to_dir(camera.dir_rad);
        camera.dir = vec3_normalize(camera.dir);

        camera.rays = (Ray**)malloc(resolution_y * sizeof(Ray*));
        
        for (int i = 0; i < resolution_y; i++)
        {
                camera.rays[i] = malloc(resolution_x * sizeof(Ray));
        }

        for (int y = 0; y < resolution_y; y++)
        {
                for (int x = 0; x < resolution_x; x++)
                {
                        camera.rays[y][x] = ray_init(camera.pos, camera.dir_deg);
                }       
        }

        return camera;
}
void camera_destroy(Camera *camera)
{
        for (int y = 0; y < camera->resolution_y; y++)
        {
                free(camera->rays[y]);
        }

        free(camera->rays);

        camera->rays = NULL;
        camera->resolution_x = 0;
        camera->resolution_y = 0;
        camera->pos.x = 0;
        camera->pos.y = 0;
        camera->pos.z = 0;
        camera->dir.x = 0;
        camera->dir.y = 0;
        camera->dir.z = 0;
}

void camera_calculate_vFOV(Camera *camera)
{
        float aspect_ratio = (float)camera->resolution_y / (float)camera->resolution_x;
        camera->vFOV = 2.0f * atan(tan(camera->hFOV / 2.0f) * aspect_ratio);
        camera->vFOV_deg = rad_to_deg(camera->vFOV);
}

void camera_calculate_ray_trajectories(Camera *camera)
{
        double horizontal_ray_step = camera->hFOV / (camera->resolution_x - 1); // The distance between rays on the x axis in radian.
        double vertical_ray_step = camera->vFOV / (camera->resolution_y - 1); // The distance between rays on the y axis in radian.

        camera->dir_rad.x = rad_normalise(camera->dir_rad.x);
        camera->dir_rad.y = rad_normalise(camera->dir_rad.y);

        double horizontal_ray_start_pos = camera->dir_rad.y - (camera->hFOV / 2); // the yaw measure in radian which rays on the x axis will start casting from, and then move right by the x distance each step.
        //double vertical_ray_start_pos = camera->dir_rad.x - (camera->vFOV / 2);
        double vertical_ray_start_pos = camera->dir_rad.x + (camera->vFOV / 2.0);

        for (int y = 0; y < camera->resolution_y; y++) // for each ray
        {
                for (int x = 0; x < camera->resolution_x; x++)
                {
                        //double ray_pitch = vertical_ray_start_pos + (camera->resolution_y - y) * vertical_ray_step; // calculate the pitch and yaw of the given ray
                        double ray_pitch = vertical_ray_start_pos - y * vertical_ray_step;
                        double ray_yaw = horizontal_ray_start_pos + x * horizontal_ray_step;

                        camera->rays[y][x].dir_rad.x = rad_normalise(ray_pitch);
                        camera->rays[y][x].dir_rad.y = rad_normalise(ray_yaw);

                        camera->rays[y][x].dir_deg.x = rad_to_deg(camera->rays[y][x].dir_rad.x);
                        camera->rays[y][x].dir_deg.y = rad_to_deg(camera->rays[y][x].dir_rad.y);

                        camera->rays[y][x].dir = dir_rad_to_dir(camera->rays[y][x].dir_rad);
                } 
        }  
}

void capture_and_pass(Camera *camera, Buffer *buffer, Map *map)
{
        if (camera->resolution_x != buffer->resolution_x || camera->resolution_y != buffer->resolution_y)
        {
                colour_print_set(COLOUR_RED);
                printf("Error: Camera resolution does not match buffer resolution");
                colour_print_reset();
                printf("\n");
                return;
        }

        for (int y = 0; y < camera->resolution_y; y++)
        {
                for (int x = 0; x < camera->resolution_x; x++)
                {
                        //ray_cast_simple(&camera->rays[y][x], map, config.max_ray_distance, config.ray_step_scalar_fine);
                        ray_cast_jump(&camera->rays[y][x], map, config.max_ray_distance, config.ray_step_scalar_coarse, config.ray_step_scalar_fine);
                        buffer->rays[y][x] = camera->rays[y][x];
                }
        }
}

void camera_rays_reset(Camera *camera)
{
        for (int y = 0; y < camera->resolution_y; y++)
        {
                for (int x = 0; x < camera->resolution_x; x++)
                {
                        camera->rays[y][x].collided = false;
                        camera->rays[y][x].collided_material = materials[MAT_UNDEFINED];
                        camera->rays[y][x].dir = camera->dir;
                        camera->rays[y][x].dir_deg = camera->dir_deg;
                        camera->rays[y][x].dir_rad = camera->dir_rad;
                        camera->rays[y][x].distance_travelled = 0;
                        camera->rays[y][x].pos = camera->pos;
                }
        }
}

void camera_print_info(Camera *camera)
{
        printf("+--- Camera Info ---------------------------------------\n");
        printf("| pos (xyz):\tx = %.2f,\ty = %.2f,\tz = %.2f\n", camera->pos.x, camera->pos.y, camera->pos.z);
        printf("| dir (xyz):\tx = %.2f,\ty = %.2f,\tz = %.2f\n", camera->dir.x, camera->dir.y, camera->dir.z);
        printf("| dir (Deg):\tPitch = %.2f,\tYaw = %.2f\n", camera->dir_deg.x, camera->dir_deg.y);
        printf("| dir (Rad):\tPitch = %.2f,\tYaw = %.2f\n", camera->dir_rad.x, camera->dir_rad.y);
        printf("| FOV (Deg):\t\th = %.2f,\tv = %.2f\n", camera->hFOV_deg, camera->vFOV_deg);
        printf("| Resolution X:\t%d\tResolution Y:\t%d\n", camera->resolution_x, camera->resolution_y);
        printf("+-------------------------------------------------------\n");
}

void camera_print_ray_trajectories_deg(Camera *camera)
{
        printf("--- Ray trajectory matrix in deg ---\n");
        for (int y = 0; y < camera->resolution_y; y++)
        {
                for (int x = 0; x < camera->resolution_x; x++)
                {
                        printf("y%d, x%d [%.1f, %.1f], ", y, x, camera->rays[y][x].dir_deg.x, camera->rays[y][x].dir_deg.y);
                }
                printf("\n");
        }  
}

void camera_print_ray_trajectories_rad(Camera *camera)
{
        printf("--- Ray trajectory matrix in rad ---\n");
        for (int y = 0; y < camera->resolution_y; y++)
        {
                for (int x = 0; x < camera->resolution_x; x++)
                {
                        printf("[%.1f, %.1f]", camera->rays[y][x].dir_rad.x, camera->rays[y][x].dir_rad.y);
                }
                printf("\n");
        }  
}
// camera.c

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/trig.h"
#include "../include/camera.h"

Camera camera_init(int resolution_x, int resolution_y, float hFOV_deg, Vec3 position, Vec2 direction_deg)
{
        Camera camera;

        camera.resolution_x = resolution_x;
        camera.resolution_y = resolution_y;

        camera.hFOV_deg = hFOV_deg;
        camera.hFOV = deg_to_rad(hFOV_deg);

        camera.position = position;

        camera.direction_deg = direction_deg;

        camera.direction_rad.x = deg_to_rad(camera.direction_deg.x);
        camera.direction_rad.y = deg_to_rad(camera.direction_deg.y);

        camera.direction = direction_rad_to_direction(camera.direction_rad);
        camera.direction = vec3_normalize(camera.direction);

        camera.rays = (Ray**)malloc(resolution_y * sizeof(Ray*));
        
        for (int i = 0; i < resolution_y; i++)
        {
                camera.rays[i] = malloc(resolution_x * sizeof(Ray));
        }

        for (int y = 0; y < resolution_y; y++)
        {
                for (int x = 0; x < resolution_x; x++)
                {
                        camera.rays[y][x] = ray_init(camera.position, camera.direction_deg);
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
        camera->position.x = 0;
        camera->position.y = 0;
        camera->position.z = 0;
        camera->direction.x = 0;
        camera->direction.y = 0;
        camera->direction.z = 0;
}

void camera_calculate_vFOV(Camera *camera)
{
        float aspect_ratio = (float)camera->resolution_y / (float)camera->resolution_x;
        camera->vFOV = 2.0f * atan(tan(camera->hFOV / 2.0f) * aspect_ratio);
        printf("%d\n", camera->vFOV);
        camera->vFOV_deg = rad_to_deg(camera->vFOV);
}

void camera_calculate_ray_trajectories(Camera *camera)
{
        double horizontal_ray_step = camera->hFOV / (camera->resolution_x - 1); // The distance between rays on the x axis in radian.
        double vertical_ray_step = camera->vFOV / (camera->resolution_y - 1); // The distance between rays on the y axis in radian.

        camera->direction_rad.x = rad_normalise(camera->direction_rad.x);
        camera->direction_rad.y = rad_normalise(camera->direction_rad.y);

        double horizontal_ray_start_position = camera->direction_rad.y - (camera->hFOV / 2); // the yaw measure in radian which rays on the x axis will start casting from, and then move right by the x distance each step.
        //double vertical_ray_start_position = camera->direction_rad.x - (camera->vFOV / 2);
        double vertical_ray_start_position = camera->direction_rad.x + (camera->vFOV / 2.0);

        for (int y = 0; y < camera->resolution_y; y++) // for each ray
        {
                for (int x = 0; x < camera->resolution_x; x++)
                {
                        //double ray_pitch = vertical_ray_start_position + (camera->resolution_y - y) * vertical_ray_step; // calculate the pitch and yaw of the given ray
                        double ray_pitch = vertical_ray_start_position - y * vertical_ray_step;
                        double ray_yaw = horizontal_ray_start_position + x * horizontal_ray_step;

                        camera->rays[y][x].direction_rad.x = rad_normalise(ray_pitch);
                        camera->rays[y][x].direction_rad.y = rad_normalise(ray_yaw);

                        camera->rays[y][x].direction_deg.x = rad_to_deg(camera->rays[y][x].direction_rad.x);
                        camera->rays[y][x].direction_deg.y = rad_to_deg(camera->rays[y][x].direction_rad.y);

                        camera->rays[y][x].direction = direction_rad_to_direction(camera->rays[y][x].direction_rad);
                } 
        }  
}

void camera_print_info(Camera *camera)
{
        printf("+--- Camera Info ---------------------------------------\n");
        printf("| Position (xyz):\tx = %.2f,\ty = %.2f,\tz = %.2f\n", camera->position.x, camera->position.y, camera->position.z);
        printf("| Direction (xyz):\tx = %.2f,\ty = %.2f,\tz = %.2f\n", camera->direction.x, camera->direction.y, camera->direction.z);
        printf("| Direction (Deg):\tPitch = %.2f,\tYaw = %.2f\n", camera->direction_deg.x, camera->direction_deg.y);
        printf("| Direction (Rad):\tPitch = %.2f,\tYaw = %.2f\n", camera->direction_rad.x, camera->direction_rad.y);
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
                        printf("y%d, x%d [%.1f, %.1f], ", y, x, camera->rays[y][x].direction_deg.x, camera->rays[y][x].direction_deg.y);
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
                        printf("[%.1f, %.1f]", camera->rays[y][x].direction_rad.x, camera->rays[y][x].direction_rad.y);
                }
                printf("\n");
        }  
}
// camera.c

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/trig.h"
#include "../include/camera.h"

Camera camera_init(int resolution_x, int resolution_y, float hFOV_deg, Vec3 position, Vec3 direction)
{
        Camera camera;
        camera.resolution_x = resolution_x;
        camera.resolution_y = resolution_y;
        camera.hFOV_deg = hFOV_deg;
        camera.hFOV = deg_to_rad(hFOV_deg);
        camera.position = position;
        camera.direction = direction;

        camera.rays = (Ray**)malloc(resolution_y * sizeof(Ray*));
        
        for (int i = 0; i < resolution_y; i++)
        {
                camera.rays[i] = malloc(resolution_x * sizeof(Ray));
        }

        for (int y = 0; y < resolution_y; y++)
        {
                for (int x = 0; x < resolution_x; x++)
                {
                        camera.rays[y][x].collided = false;
                        camera.rays[y][x].collided_material = materials[MAT_AIR];
                        camera.rays[y][x].position = camera.position;
                        camera.rays[y][x].direction = camera.direction;
                        camera.rays[y][x].distance_travelled = 0;    
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

void camera_calculate_ray_trajectories(Camera *camera);

void camera_print_info(Camera *camera)
{
        printf("--- Camera Info ---\n");
        printf("Position*:\tx=%f,\ty=%f,\tz=%f\n", camera->position.x, camera->position.y, camera->position.z);
        printf("Direction*:\tx=%f,\ty=%f,\tz=%f\n", camera->direction.x, camera->direction.y, camera->direction.z);
        printf("hFOV(deg)*:\t\t%f\tvFOV(deg)*:\t%f\n", camera->hFOV_deg, camera->vFOV_deg);
        printf("Resolution X*:\t%d\t\tResolution Y*:\t%d\n", camera->resolution_x, camera->resolution_y);
}
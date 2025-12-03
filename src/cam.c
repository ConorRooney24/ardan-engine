// cam.c

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/trig.h"
#include "../include/cam.h"

Cam cam_init(Vec3 pos, Vec2 rot, int res_x, int res_y, float hFOV)
{
        Cam cam;

        cam.res_x = res_x;
        cam.res_y = res_y;

        cam.hFOV = hFOV;

        cam.pos = pos;

        cam.rot = rot;

        cam_calculate_vFOV(&cam);

        cam.rays = (Ray**)malloc(res_y * sizeof(Ray*));
        
        for (int i = 0; i < res_y; i++)
        {
                cam.rays[i] = malloc(res_x * sizeof(Ray));
        }

        for (int y = 0; y < res_y; y++)
        {
                for (int x = 0; x < res_x; x++)
                {
                        cam.rays[y][x] = ray_init(cam.pos, cam.rot);
                }       
        }

        return cam;
}
void cam_destroy(Cam *cam)
{
        for (int y = 0; y < cam->res_y; y++)
        {
                free(cam->rays[y]);
        }

        free(cam->rays);

        cam->rays = NULL;
        cam->res_x = 0;
        cam->res_y = 0;
        cam->pos.x = 0;
        cam->pos.y = 0;
        cam->pos.z = 0;
        cam->rot.x = 0;
        cam->rot.y = 0;
        cam->vel.x = 0;
        cam->vel.y = 0;
        cam->vel.z = 0;
}

void cam_calculate_vFOV(Cam *cam)
{
        float aspect_ratio = (float)cam->res_y / (float)cam->res_x;
        cam->vFOV = 2.0f * atan(tan(cam->hFOV / 2.0f) * aspect_ratio);
}

void cam_calculate_ray_trajectories(Cam *cam)
{
        double horizontal_ray_step = cam->hFOV / (cam->res_x - 1); // The distance between rays on the x axis in radian.
        double vertical_ray_step = cam->vFOV / (cam->res_y - 1); // The distance between rays on the y axis in radian.

        cam->rot.x = rad_normalize(cam->rot.x);
        cam->rot.y = rad_normalize(cam->rot.y);

        double horizontal_ray_start_pos = cam->rot.y - (cam->hFOV / 2); // the yaw measure in radian which rays on the x axis will start casting from, and then move right by the x distance each step.
        //double vertical_ray_start_pos = cam->dir_rad.x - (cam->vFOV / 2);
        double vertical_ray_start_pos = cam->rot.x + (cam->vFOV / 2.0);

        for (int y = 0; y < cam->res_y; y++) // for each ray
        {
                for (int x = 0; x < cam->res_x; x++)
                {
                        //double ray_pitch = vertical_ray_start_pos + (cam->res_y - y) * vertical_ray_step; // calculate the pitch and yaw of the given ray
                        double ray_pitch = vertical_ray_start_pos - y * vertical_ray_step;
                        double ray_yaw = horizontal_ray_start_pos + x * horizontal_ray_step;

                        cam->rays[y][x].rot.x = rad_normalize(ray_pitch);
                        cam->rays[y][x].rot.y = rad_normalize(ray_yaw);

                        cam->rays[y][x].vel = rot_to_vel(cam->rays[y][x].rot);
                } 
        }  
}

void capture_and_pass(Cam *cam, Buffer *buffer, Map *map)
{
        if (cam->res_x != buffer->res_x || cam->res_y != buffer->res_y)
        {
                col_print_set(COL_RED);
                printf("Error: Cam resolution does not match buffer resolution");
                col_print_reset();
                printf("\n");
                return;
        }

        for (int y = 0; y < cam->res_y; y++)
        {
                for (int x = 0; x < cam->res_x; x++)
                {
                        //ray_cast_simple(&cam->rays[y][x], map, config.max_ray_distance, config.ray_step_scalar_fine);
                        ray_cast_jump(&cam->rays[y][x], map, config.max_ray_distance, config.ray_step_scalar_coarse, config.ray_step_scalar_fine);
                        buffer->rays[y][x] = cam->rays[y][x];
                }
        }
}

void cam_rays_reset(Cam *cam)
{
        for (int y = 0; y < cam->res_y; y++)
        {
                for (int x = 0; x < cam->res_x; x++)
                {
                        cam->rays[y][x].collided = false;
                        cam->rays[y][x].collided_vox.mat = materials[MAT_UNDEFINED];
                        cam->rays[y][x].collided_vox.text = texts[TEXT_UNDEFINED];
                        cam->rays[y][x].vel = cam->vel;
                        cam->rays[y][x].rot = cam->rot;
                        cam->rays[y][x].distance_traveled = 0;
                        cam->rays[y][x].pos = cam->pos;
                        cam->rays[y][x].collision_pos = cam->pos;
                        cam->rays[y][x].pos_last_alive = cam->pos;
                }
        }
}

void cam_print_info(Cam *cam)
{
        printf("+--- Cam Info ---------------------------------------\n");
        printf("| Pos (xyz):\tx = %.2f,\ty = %.2f,\tz = %.2f\n", cam->pos.x, cam->pos.y, cam->pos.z);
        printf("| Vel (xyz):\tx = %.2f,\ty = %.2f,\tz = %.2f\n", cam->vel.x, cam->vel.y, cam->vel.z);
        printf("| Rot (Rad):\tPitch = %.2f,\tYaw = %.2f\n", cam->rot.x, cam->rot.y);
        printf("| Rot (Deg):\tPitch = %.2f,\tYaw = %.2f\n", rad_to_deg(cam->rot.x), rad_to_deg(cam->rot.y));
        printf("| FOV (Rad):\t\th = %.2f,\tv = %.2f\n", cam->hFOV, cam->vFOV);
        printf("| FOV (Deg):\t\th = %.2f,\tv = %.2f\n", rad_to_deg(cam->hFOV), rad_to_deg(cam->vFOV));
        printf("| Resolution X:\t%d\tResolution Y:\t%d\n", cam->res_x, cam->res_y);
        printf("+-------------------------------------------------------\n");
}

void cam_print_ray_trajectories_deg(Cam *cam)
{
        printf("--- Ray trajectory matrix in deg ---\n");
        for (int y = 0; y < cam->res_y; y++)
        {
                for (int x = 0; x < cam->res_x; x++)
                {
                        printf("y%d, x%d [%.1f, %.1f], ", y, x, rad_to_deg(cam->rays[y][x].rot.x), rad_to_deg(cam->rays[y][x].rot.y));
                }
                printf("\n");
        }  
}

void cam_print_ray_trajectories_rad(Cam *cam)
{
        printf("--- Ray trajectory matrix in rad ---\n");
        for (int y = 0; y < cam->res_y; y++)
        {
                for (int x = 0; x < cam->res_x; x++)
                {
                        printf("[%.1f, %.1f]", cam->rays[y][x].rot.x, cam->rays[y][x].rot.y);
                }
                printf("\n");
        }  
}
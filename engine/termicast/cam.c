// cam.c

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../trig.h"
#include "cam.h"

Cam cam_init(Vec3 pos, Vec2 rot, int res_x, int res_y, float hFOV)
{
        Cam cam;

        cam.res_x = res_x;
        cam.res_y = res_y;

        cam.hFOV = hFOV;

        cam.pos = pos;

        cam.rot = rot;

        cam.vel = (Vec3){0, 0, 0};

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

void cam_calculate_ray_trajectories_legacy(Cam *cam) // old and buggy
{
        double horizontal_ray_step = cam->hFOV / (cam->res_x - 1); // The distance between rays on the x axis in radian.
        double vertical_ray_step = cam->vFOV / (cam->res_y - 1); // The distance between rays on the y axis in radian.

        cam->rot.x = rad_normalize(cam->rot.x);
        cam->rot.y = rad_normalize(cam->rot.y);

        double horizontal_ray_start_pos = cam->rot.y - (cam->hFOV / 2.0f); // the yaw measure in radian which rays on the x axis will start casting from, and then move right by the x distance each step.
        double vertical_ray_start_pos = cam->rot.x + (cam->vFOV / 2.0f);

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

void cam_calculate_ray_trajectories(Cam *cam)
{
        float tan_half_hFOV = tanf(cam->hFOV * 0.5f);
        float tan_half_vFOV = tanf(cam->vFOV * 0.5f);

        Vec3 world_up = {0.0f, 1.0f, 0.0f}; // Up relative to world space

        Vec3 cam_forward = rot_to_vel(cam->rot); // Vector representing forward relative to the cam

        Vec3 cam_right; // Vector representing right relative to the cam
        cam_right.x = cam_forward.z * world_up.y - cam_forward.y * world_up.z;
        cam_right.y = cam_forward.x * world_up.z - cam_forward.z * world_up.x;
        cam_right.z = cam_forward.y * world_up.x - cam_forward.x * world_up.y;
        cam_right = vec3_normalize(cam_right);

        Vec3 cam_up; // Vector representing up relative to the cam
        cam_up.x = cam_right.y * cam_forward.z - cam_right.z * cam_forward.y;
        cam_up.y = cam_right.z * cam_forward.x - cam_right.x * cam_forward.z;
        cam_up.z = cam_right.x * cam_forward.y - cam_right.y * cam_forward.x;
        cam_up = vec3_normalize(cam_up);

        float ndc_x, ndc_y;
        float cx, cy, cz;

        for (int y = 0; y < cam->res_y; y++)
        {
                ndc_y = (((float)y + 0.5f ) / (float)cam->res_y) * 2.0f - 1.0f;

                for (int x = 0; x < cam->res_x; x++)
                {
                        ndc_x = ( ( (float)x + 0.5f ) / (float)cam->res_x ) * 2.0f - 1.0f;

                        cx = ndc_x * tan_half_hFOV;
                        cy = ndc_y * tan_half_vFOV;
                        cz = 1.0f;

                        Vec3 vel;
                        vel.x = cx * cam_right.x + cy * cam_up.x + cz * cam_forward.x;
                        vel.y = cx * cam_right.y + cy * cam_up.y + cz * cam_forward.y;
                        vel.z = cx * cam_right.z + cy * cam_up.z + cz * cam_forward.z;

                        vel = vec3_normalize(vel);

                        cam->rays[y][x].pos = cam->pos;
                        cam->rays[y][x].vel = vel;
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

        if (config.raycast_multithreading)
        {
                pthread_t threads[cam->res_y]; // Array of threads
                RowThreadArgs args[cam->res_y]; // Array of arguments for each row thread

                for (int y = 0; y < cam->res_y; y++)
                {
                        
                        args[y].buffer = buffer;
                        args[y].cam = cam;
                        args[y].map = map;
                        args[y].row_y = y;

                        pthread_create(&threads[y], NULL, capture_and_pass_row, &args[y]); 
                }

                for (int y = 0; y < cam->res_y; y++) // join all threads back for each row
                {
                        pthread_join(threads[y], NULL);
                }
        }
        else
        {
                for (int y = 0; y < cam->res_y; y++)
                {
                        for (int x = 0; x < cam->res_x; x++)
                        {
                                //ray_cast_simple(&cam->rays[y][x], map, config.max_ray_distance, config.ray_step_scalar_fine);
                                //ray_cast_jump(&cam->rays[y][x], map, config.max_ray_distance, config.ray_step_scalar_coarse, config.ray_step_scalar_fine);
                                ray_cast_dda(&cam->rays[y][x], map, config.max_ray_distance);
                                buffer->rays[y][x] = cam->rays[y][x];
                        }
                }
        }
        
}

void *capture_and_pass_row(void *args)
{
        RowThreadArgs *a = (RowThreadArgs *)args;

        for (int x = 0; x < a->cam->res_x; x++) // For every pixel (column)  in this row
        {
                ray_cast_dda(&a->cam->rays[a->row_y][x], a->map, config.max_ray_distance);

                a->buffer->rays[a->row_y][x] = a->cam->rays[a->row_y][x];
        }

        return NULL;
}

void cam_rays_reset(Cam *cam)
{
        for (int y = 0; y < cam->res_y; y++)
        {
                for (int x = 0; x < cam->res_x; x++)
                {
                        cam->rays[y][x].collided = false;
                        cam->rays[y][x].collided_vox.mat = &materials[MAT_UNDEFINED];
                        cam->rays[y][x].collided_vox.text = &texts[TEXT_UNDEFINED];
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
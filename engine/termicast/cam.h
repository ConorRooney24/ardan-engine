// cam.h
//
// Acts as a middle man between the Raycaster and the renderer.
// Provides the cam struct. Also provides functions for calculating vFOV using hFOV as well as calculating the trajectories of each ray based on FOV and display resolution.

#ifndef CAM_H
#define CAM_H

#include "config.h"
#include "../vector.h"
#include "caster.h"
#include "renderer.h"

typedef struct Cam
{
        int res_x, res_y;
        float hFOV, vFOV;
        Vec3 pos, vel;
        Vec2 rot; // Pitch(radian from z going up), Yaw(radian from z going right); Pitch(z and y), Yaw(z and x) - a pitch of 0 and yaw of 0 meaning facing straight on towards z. a pitch of 90degrees means facing straight up, towards y. a yaw of 90 means facing right, towards x.
        Ray **rays; // y, x
} Cam;

typedef struct RowThreadArgs // Arguments passed to threads that run *capture_and_pass_row
{
        Cam *cam;
        Buffer *buffer;
        Map *map;
        int row_y;
} RowThreadArgs;

Cam cam_init(Vec3 pos, Vec2 rot, int res_x, int res_y, float hFOV);
void cam_destroy(Cam *cam);

void cam_calculate_vFOV(Cam *cam);
void cam_calculate_ray_trajectories_legacy(Cam *cam);
void cam_calculate_ray_trajectories(Cam *cam);
void capture_and_pass(Cam *cam, Buffer *buffer, Map *map); // Uses the raycaster to cast a ray for each pixel along its trajectory in the matrix and sends the ray journey information to a buffer
void *capture_and_pass_row(void *args); // Used for multithreading. same as capture_and_pass only for a specific row
void cam_rays_reset(Cam *cam); // Resets the rays for the next cast

void cam_print_info(Cam *cam);
void cam_print_ray_trajectories_deg(Cam *cam);
void cam_print_ray_trajectories_rad(Cam *cam);

#endif // CAM_H
// camera.h
//
// Acts as a middle man between the Raycaster and the renderer.
// Provides the camera struct. Also provides functions for calculating vFOV using hFOV as well as calculating the trajectories of each ray based on FOV and display resolution.

#ifndef CAMERA_H
#define CAMERA_H

#include "config.h"
#include "vector.h"
#include "caster.h"
#include "renderer.h"

typedef struct Camera
{
        int resolution_x, resolution_y;
        float hFOV, vFOV, hFOV_deg, vFOV_deg;
        Vec3 position, direction;
        Vec2 direction_rad; // Pitch(radian from z going up), Yaw(radian from z going right); Pitch(z and y), Yaw(z and x) - a pitch of 0 and yaw of 0 meaning facing straight on towards z. a pitch of 90degrees means facing straight up, towards y. a yaw of 90 means facing right, towards x.
        Vec2 direction_deg;
        Ray **rays; // y, x
} Camera;

Camera camera_init(int resolution_x, int resolution_y, float hFOV_deg, Vec3 position, Vec2 direction_deg);
void camera_destroy(Camera *camera);

void camera_calculate_vFOV(Camera *camera);
void camera_calculate_ray_trajectories(Camera *camera);
void capture_and_pass(Camera *camera, Buffer *buffer, Map *map); // Uses the raycaster to cast a ray for each pixel along its trajecory in the matrix and sends the ray journey information to a buffer

void camera_print_info(Camera *camera);
void camera_print_ray_trajectories_deg(Camera *camera);
void camera_print_ray_trajectories_rad(Camera *camera);

#endif // CAMERA_H
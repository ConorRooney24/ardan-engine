// camera.h
//
// Acts as a middle man between the Raycaster and the renderer.
// Provides the camera struct. Also provides functions for calculating vFOV using hFOV as well as calculating the trajectories of each ray based on FOV and display resolution.

#ifndef CAMERA_H
#define CAMERA_H

#include "config.h"
#include "vector.h"
#include "caster.h"

typedef struct Camera
{
        int resolution_x, resolution_y;
        float hFOV, vFOV;
        Vec3 position, direction;
        Ray **rays;
} Camera;

Camera camera_init(int resolution_x, int resolution_y, float hFOV, Vec3 position, Vec3 direction);
void camera_destroy(Camera *camera);
float camera_calculate_vFOV(Camera *camera);
void camera_calculate_ray_trajectories(Camera *camera);

#endif // CAMERA_H
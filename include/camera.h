// camera.h
//
// Acts as a middle man between the Raycaster and the renderer.
// Provides the camera struct. Also provides functions for calculating vFOV using hFOV as well as calculating the trajectories of each ray based on FOV and display resolution.

#ifndef CAMERA_H
#define CAMERA_H

#include "config.h"
#include "vector.h"

typedef struct Camera
{
        Vec3 position;
        Vec3 direction;
        char ray_trajectory_matrix[DISPLAY_WIDTH_MAX][DISPLAY_HEIGHT_MAX];
        float hFOV, vFOV;
} Camera;

float calculate_vFOV(float hFOV, int display_width, int display_height);
void calculate_ray_trajectory_matrix(Camera* camera);

#endif // CAMERA_H
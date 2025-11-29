// trig.h
//
// Trigonometry utilities

#ifndef TRIG_H
#define TRIG_H

#define TRIG_PI 3.14159265358979323846

#include "vector.h"

double rad_to_deg(double rad);
double deg_to_rad(double deg);
double vec2_to_rad(Vec2 vector); // Can be used for calculating rotation. Currently only need pitch and yaw. the vec would be sin and cos then.
Vec3 direction_rad_to_direction(Vec2 direction_rad); // Takes a pitch and yaw, gives an xyz.
Vec3 vec3_normalize(Vec3 vector);
float rad_normalise(float rad);

#endif // TRIG_H
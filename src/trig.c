// trig.c

#include <math.h>
#include "../include/trig.h"

double rad_to_deg(double rad)
{
        return rad * (180/TRIG_PI);
}

double deg_to_rad(double deg)
{
        return deg * (TRIG_PI/180);
}

double vec2_to_rad(Vec2 vector)
{
        float sin = vector.y;
        float cos = vector.x;

        return atan2(sin, cos);
}

Vec2 rad_to_vec2(double rad)
{
    Vec2 v;

    v.x = cos(rad);
    v.y = sin(rad);

    return v;
}
// vector.c

#include <math.h>
#include "../include/vector.h"

Vec2 vec2_add(Vec2 a, Vec2 b)
{
        Vec2 c;
        c.x = a.x + b.x;
        c.y = a.y + b.y;
        return c;
}

Vec3 vec3_add(Vec3 a, Vec3 b)
{
        return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec2 vec2_subtract(Vec2 a, Vec2 b)
{
        Vec2 c;
        c.x = a.x - b.x;
        c.y = a.y - b.y;
        return c;
}

Vec3 vec3_subtract(Vec3 a, Vec3 b)
{
        Vec3 c;
        c.x = a.x - b.x;
        c.y = a.y - b.y;
        c.z = a.z - b.z;
        return c;
}

Vec3 vec3_scale(Vec3 v, float scalar)
{
        return (Vec3){v.x*scalar, v.y*scalar, v.z*scalar};
}

Vec3i vec3_floor(Vec3 v)
{
        return (Vec3i){ (int)floor(v.x), (int)floor(v.y), (int)floor(v.z) };
}
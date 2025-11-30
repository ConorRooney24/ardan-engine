// vector.c

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
        Vec3 c;
        c.x = a.x + b.x;
        c.y = a.y + b.y;
        c.z = a.z + b.z;
        return c;
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
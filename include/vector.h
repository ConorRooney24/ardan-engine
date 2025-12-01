// vector.h
//
// Provides structs and functions for vectors

#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vec2
{
        float x, y;
} Vec2;

typedef struct Vec3
{
        float x, y, z;
} Vec3;

typedef struct Vec3i
{
        int x, y, z;
} Vec3i;

Vec2 vec2_add(Vec2 a, Vec2 b);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec2 vec2_subtract(Vec2 a, Vec2 b);
Vec3 vec3_subtract(Vec3 a, Vec3 b);
Vec3 vec3_scale(Vec3 v, float scalar)

#endif // VECTOR_H
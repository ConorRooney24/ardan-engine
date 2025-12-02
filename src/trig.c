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

Vec3 dir_rad_to_dir(Vec2 dir_rad)
{
        float pitch = dir_rad.x;
        float yaw = dir_rad.y;

        Vec3 dir;
        dir.x = cos(pitch) * sin(yaw);
        dir.y = sin(pitch);
        dir.z = cos(pitch) * cos(yaw);

        return dir;
}

Vec3 vec3_normalize(Vec3 vector) 
{
	float length_squared = (vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z);

	if (length_squared <= 0.0)
	{
		return (Vec3){0.0, 0.0, 0.0};
	}

	float inverse_length = 1.0f / sqrtf(length_squared);

	Vec3 result;
	result.x = vector.x * inverse_length;
	result.y = vector.y * inverse_length;
	result.z = vector.z * inverse_length;

	return result;
}

float rad_normalise(float rad)
{
	rad = fmod(rad, 2.0 * TRIG_PI);

	if(rad < 0) 
	{
		rad += 2.0 * TRIG_PI;
	}

	return rad;
}

float vec3_to_distance(Vec3 vec)
{
	return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}
// trig.c

#include "../include/trig.h"

double rad_to_deg(double rad)
{
        return rad * (180/TRIG_PI);
}

double deg_to_rad(double deg)
{
        return deg * (TRIG_PI/180);
}
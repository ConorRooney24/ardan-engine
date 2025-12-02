// config.h
//
// Provides the config struct for the entire program

#ifndef CONFIG_H
#define CONFIG_H

#define DISPLAY_WIDTH_DEFAULT 250
#define DISPLAY_HEIGHT_DEFAULT 150

#define DISPLAY_WIDTH_MAX 1920
#define DISPLAY_HEIGHT_MAX 1080

typedef struct Config
{
        int display_width, display_height;
        float hFOV, vFOV, hFOV_deg, vFOV_deg;
        float max_ray_distance, ray_step_scalar_coarse, ray_step_scalar_fine;
} Config;

extern Config config;

#endif // CONFIG_H
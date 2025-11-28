// config.h
//
// Provides the config struct for the entire program

#ifndef CONFIG_H
#define CONFIG_H

typedef struct Config
{
        int display_width, display_height;
        float hFOV, vFOV;
} Config;

extern Config config;

#endif // CONFIG_H
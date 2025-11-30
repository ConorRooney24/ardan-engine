// time.h
//
// Provides functions relating to time

#ifndef TIME_H
#define TIME_H

#define _POSIX_C_SOURCE 199309L

#include <time.h>
#include <unistd.h>

typedef struct TimeStamp
{
        struct timespec ts;
} TimeStamp;

void sleep_seconds(float seconds);

TimeStamp record_time();
TimeStamp time_diff(TimeStamp a, TimeStamp b);

#endif // TIME_H
// util_time.c

#include "util_time.h"

void sleep_ns(long ns) 
{
        struct TimeStamp a;
        a.ts.tv_sec  = ns / 1000000000LL;
        a.ts.tv_nsec = ns % 1000000000LL;

        while (nanosleep(&a.ts, &a.ts) == -1 && errno == EINTR) 
        {
                continue;
        }
}

void sleep_ms(long ms)
{
        sleep_ns(ms * 1000000.0L);
}

TimeStamp record_time()
{
        TimeStamp time;
        clock_gettime(CLOCK_MONOTONIC, &time.ts);
        return time;
}

TimeStamp time_diff(TimeStamp start, TimeStamp end)
{
        TimeStamp difference;

        difference.ts.tv_sec  = end.ts.tv_sec  - start.ts.tv_sec;
        difference.ts.tv_nsec = end.ts.tv_nsec - start.ts.tv_nsec;

        if (difference.ts.tv_nsec < 0)
        {
                difference.ts.tv_sec -= 1;
                difference.ts.tv_nsec += 1000000000LL;
        }

        return difference;
}

long long timestamp_to_ns(TimeStamp timestamp)
{
    return (long long)timestamp.ts.tv_sec * 1000000000LL + timestamp.ts.tv_nsec;
}

double timestamp_to_ms(TimeStamp timestamp)
{
    return (double)timestamp.ts.tv_sec * 1000 + (double)timestamp.ts.tv_nsec / 1000000LL;
}

double timestamp_to_s(TimeStamp timestamp)
{
    return (double)timestamp.ts.tv_sec + (double)timestamp.ts.tv_nsec / 1000000000LL;
}

float time_frame_time_to_fps(TimeStamp frametime)
{
        double frametime_seconds = timestamp_to_s(frametime);
        return 1.0L / frametime_seconds;
}
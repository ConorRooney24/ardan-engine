//time.c

#include "../include/util_time.h"

void sleep_seconds(float seconds)
{
        struct timespec ts;
        ts.tv_sec = (time_t)seconds;
        ts.tv_nsec = (seconds - ts.tv_sec) * 1e9;
        nanosleep(&ts, NULL);
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
//time.c

#include "../include/util_time.h"

void sleep_seconds(float seconds)
{
        usleep(1000000 * seconds); 
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
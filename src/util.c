#include "util.h"

long long get_timestamp_in_milliseconds(void)
{
    struct timeval time_value;

    gettimeofday(&time_value, NULL);
    long long milliseconds = time_value.tv_sec * 1000LL + time_value.tv_usec / 1000;
    
    return milliseconds;
}
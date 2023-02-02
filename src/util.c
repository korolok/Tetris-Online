#include "util.h"

void make_log(const char* log_message)
{
    time_t time_info = time(NULL);
    struct tm curr_time = *localtime(&time_info);

    FILE *file_pointer = fopen(LOG_FILE_NAME, "a");
    fprintf(
        file_pointer, 
        "[%02d:%02d:%02d] %s\n", 
        curr_time.tm_hour, curr_time.tm_min, curr_time.tm_sec, log_message
    );
    fclose(file_pointer);
}

void set_random_seed(void)
{
    srand((int)time(NULL));
}

int generate_random_number(int minval, int maxval)
{
    return (rand() % (maxval - minval + 1)) + minval;
}

long long get_timestamp_in_milliseconds(void)
{
    struct timeval time_value;

    gettimeofday(&time_value, NULL);
    long long milliseconds = time_value.tv_sec * 1000LL + time_value.tv_usec / 1000;
    
    return milliseconds;
}
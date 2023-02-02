#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define LOG_FILE_NAME "app.log"

void make_log(const char* log_message);

void    set_random_seed(void);
int     generate_random_number(int minval, int maxval);

long long get_timestamp_in_milliseconds(void);

#endif // UTIL_H
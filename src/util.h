#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#define LOG_FILE_NAME "app.log"

void    set_random_seed(void);
int     generate_random_number(int minval, int maxval);

void        make_log(const char* log_message);
long long   get_timestamp_in_milliseconds(void);
bool        get_port_from_file(int* port_buffer);

#endif // UTIL_H
#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int generate_random_number(int minval, int maxval);
long long get_timestamp_in_milliseconds(void);

#endif //   UTIL_H
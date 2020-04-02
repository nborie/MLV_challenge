#include "../../includes/util/misc.h"

#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

double get_absolute_time(void)
{

    struct timespec time_real;

    clock_gettime(CLOCK_REALTIME, &time_real);
    return ((double)time_real.tv_sec) + (((double)time_real.tv_nsec) / 1000000000L);

}


/*
 * Copyright (C) hanchong liu
 */


#include "sae_config.h"
#include "sae_core.h"

sae_short_t sae_times_timeval_cmp(struct timeval *src, struct timeval *dest)
{
    return (src->tv_sec < dest->tv_sec)   ?  1 :
           (src->tv_sec > dest->tv_sec)   ? -1 :
           (src->tv_usec < dest->tv_usec) ?  1 :
           (src->tv_usec > dest->tv_usec) ? -1 : 0;
}

void sae_times_timeval_to_timespec(struct timeval *tv, struct timespec *ts)
{
    ts->tv_sec = tv->tv_sec;
    ts->tv_nsec = tv->tv_usec * 1000;
}

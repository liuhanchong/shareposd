
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_TIMES_H_INCLUDED_
#define _SAE_TIMES_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

#if (HAVE_WIN32)
#define sae_msleep(m) Sleep(m)
#else
#define sae_msleep(m) usleep(m * (1000))
#endif

/*0-src=dest 1-< -1>*/
sae_short_t sae_times_timeval_cmp(struct timeval *src, struct timeval *dest);

void sae_times_timeval_to_timespec(struct timeval *tv, struct timespec *ts);

#endif /* _SAE_TIMES_H_INCLUDED_ */

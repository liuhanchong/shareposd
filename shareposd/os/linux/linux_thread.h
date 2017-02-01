
/*
 * Copyright (C) hanchong liu
 */


#ifndef _LINUX_THREAD_H_INCLUDED_
#define _LINUX_THREAD_H_INCLUDED_

#include "../../core/sae_type.h"

struct sae_thread_s
{
    sae_thread_id_t tid;
    sae_bool_t run;
    sae_void_t *data;
    sae_thread_task task;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

sae_thread_t *sae_thread_create(sae_thread_task task, sae_void_t *data);

sae_void_t sae_thread_destroy(sae_thread_t *thread);

sae_bool_t sae_thread_wakeup(sae_thread_t *thread);

sae_bool_t sae_thread_run(sae_thread_t *thread);

sae_void_t sae_thread_task_set(sae_thread_t *thread, sae_thread_task task, sae_void_t *data);

#endif /* _LINUX_THREAD_H_INCLUDED_ */

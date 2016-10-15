
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_THREAD_H_INCLUDED_
#define _SAE_THREAD_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

struct sae_thread_s
{
    sae_thread_id_t tid;
    sae_bool_t run;
    sae_void_t *data;
    sae_thread_task task;
#if (HAVE_WIN32)
    DWORD ident; /*Win thead id*/
    HANDLE event;/*win event*/
#else
    pthread_mutex_t mutex;
    pthread_cond_t cond;
#endif
};

sae_thread_t *sae_thread_create(sae_thread_task task, sae_void_t *data);

sae_void_t sae_thread_destroy(sae_thread_t *thread);

sae_bool_t sae_thread_wakeup(sae_thread_t *thread);

sae_bool_t sae_thread_run(sae_thread_t *thread);

sae_void_t sae_thread_task_set(sae_thread_t *thread, sae_thread_task task, sae_void_t *data);

#endif /* _SAE_THREAD_H_INCLUDED_ */

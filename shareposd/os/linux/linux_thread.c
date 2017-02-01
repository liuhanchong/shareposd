
/*
 * Copyright (C) hanchong liu
 */


#include "linux_thread.h"

static sae_void_t sae_thread_call_clean_mutex(sae_void_t *mutex)
{
    pthread_mutex_unlock(mutex);
}

static sae_void_t *sae_thread_call_run(sae_void_t *data)
{
    sae_thread_t *thread = data;
    
    /*enable thread set cancel point*/
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, sae_null);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, sae_null);
    
    while (sae_true)
    {
        /*set exception exit*/
        pthread_cleanup_push(sae_thread_call_clean_mutex, &thread->mutex);
        
        /*wait wakeup*/
        pthread_mutex_lock(&thread->mutex);
        while (!thread->run)
        {
            pthread_cond_wait((&(thread->cond)), &thread->mutex);
        }
        
        /*process work*/
        thread->task(thread->data);
        
        /*thread unwakeup*/
        thread->run = sae_false;
        
        /*set cancel point*/
        pthread_testcancel();
        
        pthread_mutex_unlock(&thread->mutex);
        
        pthread_cleanup_pop(0);
    }
    
    return sae_null;
}

sae_thread_t *sae_thread_create(sae_thread_task task, sae_void_t *data)
{
    sae_thread_t *thread = sae_malloc(sae_sizeof(sae_thread_t));
    if (!thread)
    {
        return thread;
    }
    
    thread->run  = sae_false;
    thread->task  = task;
    thread->data = data;
    
    pthread_mutex_init(&thread->mutex, sae_null);
    pthread_cond_init(&thread->cond, sae_null);
    
    if (pthread_create(&thread->tid, sae_null, sae_thread_call_run, thread) != 0)
    {
        pthread_cond_destroy(&thread->cond);
        pthread_mutex_destroy(&thread->mutex);
        
        sae_free(thread);
        
        return sae_null;
    }
    
    return thread;
}

sae_void_t sae_thread_destroy(sae_thread_t *thread)
{
    pthread_cancel(thread->tid);
    pthread_join(thread->tid, sae_null);
    
    pthread_cond_destroy(&thread->cond);
    pthread_mutex_destroy(&thread->mutex);
    
    sae_free(thread);
}

sae_bool_t sae_thread_wakeup(sae_thread_t *thread)
{
    pthread_mutex_lock(&thread->mutex);
    if (!thread->run)
    {
        thread->run = sae_true;
        pthread_cond_signal(&thread->cond);
    }
    pthread_mutex_unlock(&thread->mutex);
    
    return sae_true;
}

sae_bool_t sae_thread_run(sae_thread_t *thread)
{
    return thread->run;
}

sae_void_t sae_thread_task_set(sae_thread_t *thread, sae_thread_task task, sae_void_t *data)
{
    thread->task = task;
    thread->data = data;
}


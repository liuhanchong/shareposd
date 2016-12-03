
/*
 * Copyright (C) hanchong liu
 */


#include "sae_thread.h"

#if !(HAVE_WIN32)
static sae_void_t sae_thread_call_clean_mutex_linux(sae_void_t *mutex)
{
    pthread_mutex_unlock(mutex);
}
#endif

#if !(HAVE_WIN32)
static sae_void_t *sae_thread_call_run_linux(sae_void_t *data)
{
    sae_thread_t *thread = data;
    
    /*enable thread set cancel point*/
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, sae_null);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, sae_null);
    
    while (sae_true)
    {
        /*set exception exit*/
        pthread_cleanup_push(sae_thread_call_clean_mutex_linux, &thread->mutex);
        
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
#endif

#if (HAVE_WIN32)
static sae_void_t sae_thread_call_clean_mutex_win(sae_void_t *mutex)
{
}
#endif

#if (HAVE_WIN32)
DWORD WINAPI sae_thread_run_win(LPVOID data)
{
    sae_thread_t *thread = data;
    
    while (WaitForSingleObject( thread->event, INFINITE) == WAIT_OBJECT_0)
    {
        /*process work*/
        thread->task(thread->data);
        
        /*thread unwakeup*/
        thread->run = sae_false;
    }
    
    return 0;
}
#endif

sae_thread_t *sae_thread_create(sae_thread_task task, sae_void_t *data)
{
    sae_thread_t *thread = sae_alloc(sae_sizeof(sae_thread_t));
    if (!thread)
    {
        return thread;
    }
    
    thread->run  = sae_false;
    thread->task  = task;
    thread->data = data;
    
#if (HAVE_WIN32)
    
    thread->event = CreateEvent(sae_null, FALSE, FALSE, sae_null);/*auto reset event*/
    
    thread->tid = CreateThread(sae_null,               /*default security attributes*/
                               0,                  /*use default stack size*/
                               sae_thread_run_win, /*hread function name*/
                               thread,             /*argument to thread function*/
                               0,                  /*use default creation flags*/
                               &thread->ident);
    if (thread->tid == sae_null)
    {
        CloseHandle(thread->event);
        
        sae_alloc_free(thread);
        
        return sae_null;
    }
#else
    pthread_mutex_init(&thread->mutex, sae_null);
    pthread_cond_init(&thread->cond, sae_null);
    
    if (pthread_create(&thread->tid, sae_null, sae_thread_call_run_linux, thread) != 0)
    {
        pthread_cond_destroy(&thread->cond);
        pthread_mutex_destroy(&thread->mutex);
        
        sae_alloc_free(thread);
        
        return sae_null;
    }
#endif
    
    return thread;
}

sae_void_t sae_thread_destroy(sae_thread_t *thread)
{
#if (HAVE_WIN32)
    sae_uint_t count = 0;
    /*wait thread run stop*/
    while (thread->run && ((count++ <= 100)));
    CloseHandle(thread->event);
    CloseHandle(thread->tid);
#else
    pthread_cancel(thread->tid);
    pthread_join(thread->tid, sae_null);
    
    pthread_cond_destroy(&thread->cond);
    pthread_mutex_destroy(&thread->mutex);
#endif
    
    sae_alloc_free(thread);
}

sae_bool_t sae_thread_wakeup(sae_thread_t *thread)
{
#if (HAVE_WIN32)
    if (!thread->run)
    {
        thread->run = sae_true;
        SetEvent(thread->event);
    }
#else
    pthread_mutex_lock(&thread->mutex);
    if (!thread->run)
    {
        thread->run = sae_true;
        pthread_cond_signal(&thread->cond);
    }
    pthread_mutex_unlock(&thread->mutex);
#endif
    
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


/*
 * Copyright (C) hanchong liu
 */


#ifndef _WIN_THREAD_H_INCLUDED_
#define _WIN_THREAD_H_INCLUDED_

struct sae_thread_s
{
    sae_thread_id_t tid;
    sae_bool_t run;
    sae_void_t *data;
    sae_thread_task task;
    DWORD ident; /*Win thead id*/
    HANDLE event;/*win event*/
};

static DWORD WINAPI sae_thread_run(LPVOID data)
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
    
    thread->event = CreateEvent(sae_null, FALSE, FALSE, sae_null);/*auto reset event*/
    
    thread->tid = CreateThread(sae_null,               /*default security attributes*/
                               0,                  /*use default stack size*/
                               sae_thread_run, /*hread function name*/
                               thread,             /*argument to thread function*/
                               0,                  /*use default creation flags*/
                               &thread->ident);
    if (thread->tid == sae_null)
    {
        CloseHandle(thread->event);
        
        sae_alloc_free(thread);
        
        return sae_null;
    }
    
    return thread;
}

sae_void_t sae_thread_destroy(sae_thread_t *thread)
{
    sae_uint_t count = 0;
    /*wait thread run stop*/
    while (thread->run && ((++count <= 100)));
    CloseHandle(thread->event);
    CloseHandle(thread->tid);
    
    sae_alloc_free(thread);
}

sae_bool_t sae_thread_wakeup(sae_thread_t *thread)
{
    if (!thread->run)
    {
        thread->run = sae_true;
        SetEvent(thread->event);
    }
    
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

#endif /* _WIN_THREAD_H_INCLUDED_ */

//
//  test_sae_thread.h
//  shareposd
//
//  Created by liuhanchong on 2016/10/4.
//  Copyright © 2016年 liuhanchong. All rights reserved.
//

#ifndef test_sae_thread_h
#define test_sae_thread_h

sae_void_t *task(void *data)
{
    int i = 0;
    for (i = 1; i <= 100; i++)
    {
        printf("%d ", i);
    }
    printf(" data = %d \n\n", *(int *)data);
    
    return sae_null;
}

sae_void_t *task1(void *data)
{
    int i = 0;
    for (i = 1; i <= 100; i++)
    {
        printf("%d ", i);
    }
    printf(" data = %d \n\n", *(int *)data);
    
    return sae_null;
}

sae_int_t main_thread(sae_int_t argc, sae_char_t **argv)
{
    int i = 100;
    int count = 10000000;
    
    sae_thread_t *thread = sae_thread_create(task, &i);
    if (!thread)
    {
        printf("create failed\n");
        return 1;
    }
    
    while (count-- > 0);
    
    if (!sae_thread_run(thread))
    {
        printf("no run1\n");
        
        sae_thread_wakeup(thread);
    }
    
    count = 10000000;
    while (count-- > 0);
    
    if (!sae_thread_run(thread))
    {
        printf("no run2\n");
        
        i = 101;
        
        sae_thread_task_set(thread, task1, &i);
        
        sae_thread_wakeup(thread);
    }
    
    count = 100000000;
    while (count-- > 0);
    
    sae_thread_destroy(thread);
    
    printf("ok\n");
    
    return 0;
}

#endif /* test_sae_thread_h */

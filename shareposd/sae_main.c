
/*
 * Copyright (C) hanchong liu
 */


#include "./event/sae_event.h"

sae_bool_t handle(sae_event_t *event)
{
    event->event_call(event, event->event_call_arg);
    
    return sae_true;
}

sae_int_t main(sae_int_t argc, sae_char_t **argv)
{
    //create deamon process
    if (!sae_utils_process_deamon())
    {
        sae_log(LDEBUG, "create deamon process failed");
        return EXIT_FAILURE;
    }
    
    sae_log(LDEBUG, "create deamon process success");
    
    sae_event_top_array_t *array = sae_event_top_create();
    if (!array)
    {
        sae_log(LERROR, "sae_event_top_create failed");
        return EXIT_FAILURE;
    }
    
    sae_log(LDEBUG, "the demon pid is %d", sae_res_pid_get());
    
    sae_log(LDEBUG, "event top num is %d", array->nelts);
    
    sae_event_base_t *base = sae_event_base_create(sae_array_value_get(array, 0), handle);
    if (!base)
    {
        sae_log(LERROR, "sae_event_base_create failed");
        return EXIT_FAILURE;
    }
    
    if (!sae_event_base_dispatch(base))
    {
        sae_log(LERROR, "sae_event_base_dispatch failed");
    }
    
    sae_event_base_destroy(base);
    
    sae_event_top_destroy(array);
    
    sae_log(LEXIT, "exist success");
    
    return EXIT_SUCCESS;
}

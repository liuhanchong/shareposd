
/*
 * Copyright (C) hanchong liu
 */


#include "sae_config.h"
#include "sae_core.h"
#include "sae_event.h"

sae_bool_t handle(sae_event_t *event)
{
    return sae_true;
}

sae_int_t main(sae_int_t argc, sae_char_t **argv)
{
    sae_event_top_array_t *array = sae_event_top_create();
    if (!array)
    {
        printf("sae_event_top_create failed\n");
        return 1;
    }
    
    printf("event top num is %d\n", array->nelts);
    
    sae_event_base_t *base = sae_event_base_create(sae_array_value_get(array, 0), handle);
    if (!base)
    {
        printf("sae_event_base_create failed\n");
        return 1;
    }
    
    if (!sae_event_base_dispatch(base))
    {
        printf("sae_event_base_dispatch failed\n");
    }
    
    sae_event_base_destroy(base);
    
    sae_event_top_destroy(array);
    
    return 0;
}

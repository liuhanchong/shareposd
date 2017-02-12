
/*
 * Copyright (C) hanchong liu
 */


#include "http/sae_http.h"

sae_int_t main(sae_int_t argc, sae_char_t **argv)
{
    //create deamon process
    if (!sae_utils_process_deamon())
    {
        sae_log(LERROR, "create deamon process failed");
        return EXIT_FAILURE;
    }
    
    sae_log(LDEBUG, "create deamon process success, the demon pid is %d", sae_res_pid_get());
    
    sae_event_top_array_t *array = sae_event_top_create();
    if (!array)
    {
        sae_log(LERROR, "sae_event_top_create failed");
        return EXIT_FAILURE;
    }
    
    if (array->nelts < 1)
    {
        sae_log(LERROR, "event top num is %d, sys must have one event top", array->nelts);
        return EXIT_FAILURE;
    }
    
    sae_log(LDEBUG, "event top num is %d", array->nelts);
    
    sae_cycle_conf_t *cycle_conf = sae_cycle_conf_create();
    if (!cycle_conf)
    {
        sae_log(LERROR, "sae_cycle_conf_create failed");
        return EXIT_FAILURE;
    }
    
    sae_http_t *http = sae_http_create(cycle_conf);
    if (!http)
    {
        sae_log(LERROR, "sae_event_base_create failed");
        return EXIT_FAILURE;
    }
    
    if (!sae_http_run(http))
    {
        sae_log(LERROR, "sae_event_base_dispatch failed");
    }
    
    sae_http_destroy(http);
    
    sae_event_top_destroy(array);
    
    sae_cycle_conf_destroy(cycle_conf);
    
    sae_log(LEXIT, "exist success");
    
    return EXIT_SUCCESS;
}

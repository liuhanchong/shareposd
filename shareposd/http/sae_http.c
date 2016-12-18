
/*
 * Copyright (C) hanchong liu
 */


#include "sae_http.h"

static sae_bool_t sae_http_handle(sae_event_t *event)
{
    sae_void_t *ret = sae_null;
    sae_http_packet_t *packet = sae_null;
    
    if (!(event->event_state & SAE_EVENT_STATE_ACTIVE))
    {
        return sae_false;
    }
    
    ret = event->event_call(event, event->event_call_arg);
    
    /*read data need process*/
    if ((event->event_flag & SAE_EVENT_READ) && ret)
    {
        packet = ret;
    }
    
    //no persist event del
    if (!(event->event_flag & SAE_EVENT_PERSIST))
    {
        sae_event_del(event);
        
        if (event->event_flag & SAE_EVENT_READ ||
            event->event_flag & SAE_EVENT_WRITE)
        {
            sae_socket_close(event->event_fd);
        }
    }
    
    return sae_true;
}

sae_http_t *sae_http_create(sae_cycle_conf_t *cycle)
{
    sae_http_handle(sae_null);
    
    return sae_null;
}

sae_bool_t sae_http_destroy(sae_http_t *http)
{
    return sae_true;
}

sae_bool_t sae_http_run(sae_http_t *http)
{
    return sae_true;
}



/*
 * Copyright (C) hanchong liu
 */


#include "sae_http.h"

static sae_bool_t sae_http_handle(sae_event_t *event)
{
    sae_http_packet_t *packet = sae_null;
    
    if (!(event->event_state & SAE_EVENT_STATE_ACTIVE))
    {
        return sae_false;
    }
    
    packet = event->event_call(event, event->event_call_arg);
    
    /*read data need process*/
    if ((event->event_flag & SAE_EVENT_READ) && packet)
    {
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


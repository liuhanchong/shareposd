
/*
 * Copyright (C) hanchong liu
 */


#include "sae_http.h"

static sae_bool_t sae_http_handle(sae_event_t *event)
{
    sae_void_t *ret = sae_null;
    
    if (!(event->event_state & SAE_EVENT_STATE_ACTIVE))
    {
        return sae_false;
    }
    
    ret = event->event_call(event, event->event_call_arg);
    
    return sae_true;
}

sae_http_t *sae_http_create(sae_cycle_conf_t *cycle_conf)
{
    assert(cycle_conf);
    
    sae_http_t *http = sae_null;
    sae_uint_t i = 0;
    sae_connection_listening_t *listen = sae_null;
    sae_uint_t fd_max = (sae_uint_t)sae_res_fd_max_get();
    sae_http_server_t *http_server = sae_null;
    sae_sin_addr_t addr;
    sae_in_port_t port = 0;
    
    http = sae_alloc(sae_sizeof(http));
    if (!http)
    {
        return http;
    }
    
    http->http_server_listens = sae_array_create(1, sae_sizeof(sae_connection_listening_t));
    if (!http->http_server_listens)
    {
        return sae_null;
    }
    
    http->http_servers = sae_array_create(1, sae_sizeof(sae_http_server_t));
    if (!http->http_servers)
    {
        return sae_null;
    }
    
    sae_socket_aton(cycle_conf->cycle_core->ip, &addr);
    port = sae_socket_htons(cycle_conf->cycle_core->port);
    
    for (i = 0; i < http->http_server_listens->nelts; ++i)
    {
        listen = sae_array_push_index(http->http_server_listens, i);
        http_server = sae_array_push_index(http->http_servers, i);
        
        http_server->http = http;
        http_server->http_listen = listen;
        
        if (!sae_listening_socket_inet_stream(listen, addr.s_addr, port, sae_true))
        {
            return sae_null;
        }
        
        if (!sae_listening_socket_open(listen))
        {
            return sae_null;
        }
        
        http_server->http_requset_packet_table = sae_http_packet_table_create(fd_max);
        if (!http_server->http_requset_packet_table)
        {
            return sae_null;
        }
        
        http_server->http_server_event_base = sae_event_base_create(sae_array_value_get(cycle_conf->cycle_core->event_top_array, 0), sae_http_handle);
        if (!http_server->http_server_event_base)
        {
            return sae_null;
        }
        
        if (!(http_server->http_server_event_socket_accept = sae_event_socket_set(http_server->http_server_event_base, http_server->http_listen->fd, SAE_EVENT_READ | SAE_EVENT_PERSIST, sae_http_request_accept, http_server)))
        {
            return sae_null;
        }
        
        if (!sae_event_socket_add(http_server->http_server_event_socket_accept))
        {
            return sae_null;
        }
    }
    
    return http;
}

sae_bool_t sae_http_destroy(sae_http_t *http)
{
    sae_uint_t i = 0;
    sae_http_server_t *http_server = sae_null;
    
    for (i = 0; i < http->http_servers->nelts; ++i)
    {
        http_server = sae_array_value_get(http->http_servers, i);
        
        sae_event_free(http_server->http_server_event_socket_accept);
        sae_listening_socket_close(http_server->http_listen);
        sae_http_packet_table_destroy(http_server->http_requset_packet_table);
        sae_event_base_destroy(http_server->http_server_event_base);
    }
    
    sae_array_destroy(http->http_server_listens);
    sae_array_destroy(http->http_servers);
    sae_alloc_free(http);
    
    return sae_true;
}

sae_bool_t sae_http_run(sae_http_t *http)
{
    sae_http_server_t *http_server = sae_array_value_get(http->http_servers, 0);
    return sae_event_base_dispatch(http_server->http_server_event_base);
}


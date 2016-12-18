
/*
 * Copyright (C) hanchong liu
 */


#include "sae_http_request.h"

static sae_bool_t sae_http_request_valid(sae_http_packet_t *packet)
{
    return sae_true;
}

static sae_http_packet_t *sae_http_request_packet_get(sae_http_t *http_server, sae_event_t *event)
{
    sae_http_packet_t *packet = sae_null;
    
    if ((packet = sae_table_value(http_server->packet_table->packet, event->event_fd_str)))
    {
        return packet;
    }
    
    /*add new packet*/
    if (!(packet = sae_http_packet_create(event->event_fd, http_server->packet_table)))
    {
        sae_log(LERROR, "sae_http_read create new packet failed, the client fd is %d", event->event_fd);
        return sae_null;
    }
    
    if (!sae_http_packet_push(packet))
    {
        sae_log(LERROR, "sae_http_read push to packet table failed, the client fd is %d", event->event_fd);
        return sae_null;
    }
    
    return packet;
}

sae_void_t *sae_http_request_accept(sae_event_t *event, sae_void_t *arg)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sae_sizeof(client_addr);
    sae_http_t *http_server = arg;
    sae_event_t *http_client_event = sae_null;
    sae_socket_fd_t http_client_fd = -1;
    
    if (http_server->http_server_fd != event->event_fd)
    {
        return sae_null;
    }

    if ((http_client_fd = sae_socket_accept(http_server->http_server_fd, (struct sockaddr *)&client_addr, client_addr_len)) < 0)
    {
        sae_log(LERROR, "sae_http_accept->sae_socket_accept failed, server fd is %d", http_server->http_server_fd);
        return sae_null;
    }
    
    //set fd nonblock
    sae_socket_nonblock(http_client_fd);
    
    sae_log(LDEBUG, "accept client info is %s, client fd is %d", sae_socket_addr(&client_addr, client_addr_len), http_client_fd);
    
    if (!(http_client_event = sae_event_socket_set(event->event_base, http_client_fd, SAE_EVENT_READ | SAE_EVENT_PERSIST, sae_http_request_read, http_server)))
    {
        sae_log(LERROR, "sae_http_accept->sae_event_socket_set failed client fd is %d", http_client_fd);
        return sae_null;
    }
    
    if (!sae_event_socket_add(http_client_event))
    {
        sae_event_free(http_client_event);
        sae_log(LERROR, "sae_http_accept->sae_event_socket_add failed client fd is %d", http_client_fd);
        return sae_null;
    }
    
    return sae_null;
}

sae_void_t *sae_http_request_read(sae_event_t *event, sae_void_t *arg)
{
    sae_http_t *http_server = arg;
    sae_http_packet_t *packet = sae_null;
    
    /*get packet*/
    if (!(packet = sae_http_request_packet_get(http_server, event)))
    {
        return sae_null;
    }
    
    /*read data*/
    if (!sae_http_packet_read(packet))
    {
        sae_log(LERROR, "sae_http_read read packet data failed, the client fd is %d", event->event_fd);
        return sae_null;
    }
    
    /*valid packet*/
    if (!sae_http_request_valid(packet))
    {
        return sae_null;
    }
    
    /*remove packet*/
    if (!sae_http_packet_del(packet))
    {
        sae_log(LERROR, "sae_http_read del packet failed, the client fd is %d", event->event_fd);
        return sae_null;
    }
    
    return packet;
}

sae_void_t *sae_http_request_write(sae_event_t *event, sae_void_t *arg)
{
    return sae_null;
}

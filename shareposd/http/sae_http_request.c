
/*
 * Copyright (C) hanchong liu
 */


#include "sae_http_request.h"
#include "tp/klhttp-in.h"

static struct http_request *sae_http_request_valid(sae_http_packet_t *packet)
{
    struct http_request *request = http_request_parse(packet->http_buffer);
    if (!request)
    {
        return request;
    }
    
    sae_int_t len = atoi(http_get_header_value(request->headers, "Content-Length"));
    if (packet->http_buffer->buffer_off == len)
    {
        return request;
    }
    
    if (packet->http_buffer->buffer_off > len)
    {
        sae_log(LERROR, "sae_http_request_valid appear packet splicing");
    }
    
    http_request_free(request);
    return sae_null;
}

static sae_http_packet_t *sae_http_request_packet_get(sae_http_t *http_server, sae_event_t *event)
{
    sae_http_packet_t *packet = sae_null;
    
    if ((packet = sae_table_value(http_server->requset_packet_table->packet, event->event_fd_str)))
    {
        return packet;
    }
    
    /*add new packet*/
    if (!(packet = sae_http_packet_create(event->event_fd, http_server->requset_packet_table)))
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

static sae_bool_t sae_http_request_parse(sae_http_packet_t *request_packet, sae_buffer_t *response_buffer, struct http_request *request)
{
    if(check_request_valid(response_buffer, request) != 0)
    {
        sae_log(LERROR, "sae_http_request_packet_parse check_request_valid failed");
        return sae_false;
    }
    
    if (http_add_body(request->headers, request_packet->http_buffer) != 0)
    {
        sae_log(LERROR, "sae_http_request_packet_parse http_add_body failed");
        return sae_false;
    }
    
    return sae_true;
}

static sae_void_t sae_http_response_head_add(sae_buffer_t *response_buffer, const struct http_request *request, sae_char_t *type)
{
    /* http version info */
    sae_buffer_add_printf(response_buffer, "HTTP/%d.%d %d %s\r\n", request->ver.major,
                        request->ver.minor, HTTP_OK, CODE_STR(HTTP_OK));
    
    /* server info */
    sae_buffer_add_printf(response_buffer, "Server: sharepos/0.1.0\r\n");
    
    add_time_header(response_buffer);
    
    sae_buffer_add_printf(response_buffer, "Content-Type: %s\r\n", type);
}

static sae_void_t sae_http_response_body_add(sae_buffer_t *response_buffer, sae_buffer_t *result_buffer)
{
    sae_buffer_add_printf(response_buffer, "Content-Length: %lu\r\n", result_buffer->buffer_off);
    
    sae_buffer_add(response_buffer, "\r\n", 2);
    
    sae_buffer_add_printf(response_buffer, "%s", result_buffer->buffer);
}

static sae_bool_t sae_http_response(sae_event_base_t *base, sae_socket_fd_t fd, sae_buffer_t *buffer)
{
    sae_event_t *http_client_event = sae_null;
    
    /*this copy buffer in send success sae_http_request_write fun free*/
    sae_buffer_t *copy_buffer = sae_buffer_copy(buffer);
    
    if (!(http_client_event = sae_event_socket_set(base, fd, SAE_EVENT_WRITE | SAE_EVENT_PERSIST, sae_http_request_write, copy_buffer)))
    {
        sae_log(LERROR, "sae_http_request_write sae_event_socket_set failed client fd is %d", fd);
        return sae_false;
    }
    
    if (!sae_event_socket_add(http_client_event))
    {
        sae_event_free(http_client_event);
        sae_log(LERROR, "sae_http_request_write sae_event_socket_add failed client fd is %d", fd);
        return sae_false;
    }
    
    return sae_true;
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
    
    /*set fd nonblock*/
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
    struct http_request *request = sae_null;
    sae_http_packet_t *request_packet = sae_null;
    sae_buffer_t *response_buffer = sae_null;
    sae_buffer_t *result_buffer = sae_null;
    
    /*get packet*/
    if (!(request_packet = sae_http_request_packet_get(http_server, event)))
    {
        return sae_null;
    }
    
    /*read data*/
    if (!sae_http_packet_read(request_packet))
    {
        sae_log(LERROR, "sae_http_read read packet data failed, the client fd is %d", event->event_fd);
        return sae_null;
    }
    
    /*valid packet*/
    if (!(request = sae_http_request_valid(request_packet)))
    {
        return sae_null;
    }
    
    response_buffer = sae_buffer_create();
    if (!response_buffer)
    {
        http_request_free(request);
        sae_log(LERROR, "sae_http_read sae_buffer_create failed, the client fd is %d", event->event_fd);
        return response_buffer;
    }
    
    /*remove packet*/
    if (!sae_http_packet_del(request_packet))
    {
        http_request_free(request);
        sae_buffer_destroy(response_buffer);
        sae_log(LERROR, "sae_http_read del packet failed, the client fd is %d", event->event_fd);
        return sae_null;
    }
    
    if (!sae_http_request_parse(request_packet, response_buffer, request))
    {
        http_request_free(request);
        sae_http_packet_destroy(request_packet);
        sae_buffer_destroy(response_buffer);
        sae_log(LERROR, "sae_http_read parse packet failed, the client fd is %d", event->event_fd);
        return sae_null;
    }
    
    /*into logic*/
    result_buffer = sae_null;
    if (!result_buffer)
    {
        sae_log(LERROR, "sae_http_read logic process failed, the client fd is %d", event->event_fd);
    }

    sae_http_response_head_add(response_buffer, request, "application/json");

    sae_http_response_body_add(response_buffer, result_buffer);
    
    if (!sae_http_response(event->event_base, event->event_fd, response_buffer))
    {
        sae_log(LERROR, "sae_http_read sae_http_request_write failed, the client fd is %d", event->event_fd);
    }
    
    http_request_free(request);
    sae_buffer_destroy(result_buffer);
    sae_http_packet_destroy(request_packet);
    sae_buffer_destroy(response_buffer);
    
    return sae_null;
}

sae_void_t *sae_http_request_write(sae_event_t *event, sae_void_t *arg)
{
    sae_buffer_t *buffer = arg;
    
    sae_ssize_t len = sae_socket_send(event->event_fd, buffer->buffer, buffer->buffer_off, 0);
    if (len <= 0)
    {
        sae_buffer_destroy(buffer);
        
        if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
        {
            return sae_null;
        }
        
        sae_socket_close(event->event_fd);
        sae_log(LERROR, "sae_http_request_write sae_socket_send failed");
        return sae_null;
    }
    
    sae_buffer_destroy(buffer);
    
    return sae_null;
}

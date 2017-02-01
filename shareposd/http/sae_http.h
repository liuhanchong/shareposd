
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_HTTP_H_INCLUDED_
#define _SAE_HTTP_H_INCLUDED_

#include "../event/sae_event.h"

typedef struct sae_http_s sae_http_t;
typedef struct sae_http_server_s sae_http_server_t;
typedef struct http_request http_request_t;
typedef struct sae_http_packet_s sae_http_packet_t;
typedef struct sae_http_packet_table_s sae_http_packet_table_t;

struct sae_http_packet_s
{
    sae_socket_fd_t http_fd;
    sae_http_packet_table_t *packet_table;
    sae_char_t *http_fd_str;
    sae_buffer_t *http_buffer;
};

struct sae_http_packet_table_s
{
    sae_table_t *packet;
};

struct sae_http_server_s
{
    sae_http_t *http;
    sae_http_packet_table_t *http_requset_packet_table;
    sae_event_base_t *http_server_event_base;
    sae_connection_listening_t *http_listen;
    sae_event_t *http_server_event_socket_accept;
};

struct sae_http_s
{
    sae_array_t *http_server_listens;
    sae_array_t *http_servers;
};

#include "sae_http_packet.h"
#include "sae_http_request.h"
#include "tp/klhttp-in.h"

sae_http_t *sae_http_create(sae_cycle_conf_t *cycle_conf);

sae_bool_t sae_http_destroy(sae_http_t *http);

sae_bool_t sae_http_run(sae_http_t *http);

#endif /* _SAE_HTTP_H_INCLUDED_ */

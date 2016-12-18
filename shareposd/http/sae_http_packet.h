
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_HTTP_PACKET_H_INCLUDED_
#define _SAE_HTTP_PACKET_H_INCLUDED_

#include "../core/sae_config.h"
#include "../core/sae_core.h"

typedef struct sae_http_packet_s sae_http_packet_t;
typedef struct sae_http_packet_table_s sae_http_packet_table_t;

struct sae_http_packet_s
{
    sae_socket_fd_t http_client_fd;
    sae_http_packet_table_t *packet_table;
    sae_char_t *http_client_fd_str;
    sae_buffer_t *http_client_buffer;
};

struct sae_http_packet_table_s
{
    sae_table_t *packet;
};

sae_http_packet_t *sae_http_packet_create(sae_socket_fd_t fd, sae_http_packet_table_t *table);

sae_void_t sae_http_packet_destroy(sae_http_packet_t *packet);

sae_bool_t sae_http_packet_read(sae_http_packet_t *packet);

sae_bool_t sae_http_packet_push(sae_http_packet_t *packet);

sae_bool_t sae_http_packet_del(sae_http_packet_t *packet);

#endif /* _SAE_HTTP_PACKET_H_INCLUDED_ */


/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_HTTP_PACKET_H_INCLUDED_
#define _SAE_HTTP_PACKET_H_INCLUDED_

#include "sae_http.h"

sae_http_packet_t *sae_http_packet_create(sae_socket_fd_t fd, sae_http_packet_table_t *table);

sae_void_t sae_http_packet_destroy(sae_http_packet_t *packet);

sae_bool_t sae_http_packet_read(sae_http_packet_t *packet);

sae_bool_t sae_http_packet_push(sae_http_packet_t *packet);

sae_bool_t sae_http_packet_del(sae_http_packet_t *packet);

sae_http_packet_table_t *sae_http_packet_table_create(sae_uint_t len);

sae_void_t sae_http_packet_table_destroy(sae_http_packet_table_t *table);

#endif /* _SAE_HTTP_PACKET_H_INCLUDED_ */

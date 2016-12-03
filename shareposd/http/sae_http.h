
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_HTTP_H_INCLUDED_
#define _SAE_HTTP_H_INCLUDED_

#include "../event/sae_event.h"
#include "sae_http_request.h"
#include "sae_http_packet.h"

typedef struct sae_http_s sae_http_t;

struct sae_http_s
{
    sae_socket_fd_t http_server_fd;
    sae_http_packet_table_t *packet_table;
};

sae_http_t *sae_http_create(sae_cycle_conf_t *cycle);

sae_bool_t sae_http_destroy(sae_http_t *http);

sae_bool_t sae_http_run(sae_http_t *http);

#endif /* _SAE_HTTP_H_INCLUDED_ */

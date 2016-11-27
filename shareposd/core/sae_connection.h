
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_CONNECTION_H_INCLUDED_
#define _SAE_CONNECTION_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

struct sae_connection_listening_s
{
    sae_socket_fd_t fd;

    struct sockaddr *sock_addr;
    
    socklen_t sock_len;    /* size of sockaddr */
    
    sae_int_t addr; /* offset to address in sockaddr */
    sae_int_t addr_text_max_len;
    sae_char_t *addr_text;
    sae_int_t addr_text_len;

    sae_int_t               family;
    sae_int_t               type;
    sae_int_t               protocol;
    sae_int_t               flags;      /* Winsock2 flags */

    sae_void_t (*handler)(sae_connection_t *c); /* handler of accepted connection */
    sae_void_t *ctx;        /* sae_http_conf_ctx_t, for example */
    sae_void_t *servers;    /* array of sae_http_in_addr_t, for example */

    sae_int_t back_log;

    sae_size_t post_accept_buffer_size; /* should be here because
                                                  of the AcceptEx() preread */
    time_t post_accept_timeout;     /* should be here because
                                                  of the deferred accept */
    
    sae_bool_t ignore; /*no use*/
};

struct sae_connection_s
{
    sae_void_t *data;
    
    sae_void_t *ctx;
    sae_void_t *servers;

    struct sockaddr *sock_addr;
    socklen_t sock_len;
    sae_char_t *addr_text;
};

sae_connection_listening_t *sae_listening_inet_stream_socket(sae_cycle_core_t *cycle, sae_in_addr_t addr, sae_in_port_t port);

sae_bool_t sae_open_listening_sockets(sae_cycle_core_t *cycle);

sae_void_t sae_close_listening_sockets(sae_cycle_core_t *cycle);

#endif /* _SAE_CONNECTION_H_INCLUDED_ */

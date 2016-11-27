
/*
 * Copyright (C) hanchong liu
 */


#include "sae_config.h"
#include "sae_core.h"

sae_connection_listening_t *sae_listening_inet_stream_socket(sae_cycle_core_t *cycle,
                                                             sae_in_addr_t addr, sae_in_port_t port)
{
    sae_size_t len = 0;
    sae_connection_listening_t *listen = sae_null;
    struct sockaddr_in *addr_in = sae_null;

    listen = sae_array_push(cycle->listens);
    if (!listen)
    {
        return listen;
    }
    
    sae_memzero(listen, sae_sizeof(sae_connection_listening_t));

    if (!(addr_in = sae_alloc(sae_sizeof(struct sockaddr_in))))
    {
        sae_alloc_free(listen);
        return sae_null;
    }

#if (HAVE_SIN_LEN)
    addr_in->sin_len = sae_sizeof(struct sockaddr_in);
#endif
    addr_in->sin_family = AF_INET;
    addr_in->sin_addr.s_addr = addr;
    addr_in->sin_port = htons(port);

    if (!(listen->addr_text = sae_alloc(INET_ADDRSTRLEN + 6)))
    {
        sae_alloc_free(addr_in);
        sae_alloc_free(listen);
        return sae_null;
    }

    /*combine 192.168.10.1:1234*/
    len = sae_strlen(inet_ntop(AF_INET, &addr, listen->addr_text, INET_ADDRSTRLEN));
    listen->addr_text_len = sae_snprintf(listen->addr_text + len, 6, ":%d", port);

    /*init socket option*/
    listen->fd = (sae_socket_fd_t)-1;
    listen->family = AF_INET;
    listen->type = SOCK_STREAM;
    listen->protocol = IPPROTO_IP;
    
#if (HAVE_WIN32)
    listen->flags = WSA_FLAG_OVERLAPPED;
#endif
    listen->sock_addr = (struct sockaddr *)addr_in;
    listen->sock_len = sizeof(struct sockaddr_in);
    listen->addr = offsetof(struct sockaddr_in, sin_addr);
    listen->addr_text_max_len = INET_ADDRSTRLEN;
    
    /*use*/
    listen->ignore = sae_false;

    return listen;
}

sae_bool_t sae_open_listening_sockets(sae_cycle_core_t *cycle)
{
    sae_uint_t tries = 0;/*try number*/
    sae_uint_t failed = 0;
    sae_uint_t reuseaddr = 1;
    sae_uint_t i = 0;
    sae_socket_fd_t s = (sae_socket_fd_t)-1;
    sae_connection_listening_t  *listens = sae_null;

    /* tries configurable */
    for (tries = /* STUB */ 5; tries > 0; tries--)
    {
        failed = 0;

        /* for each listening socket */
        listens = cycle->listens->elts;
        for (i = 0; i < cycle->listens->nelts; i++)
        {
            if (listens[i].ignore ||
                listens[i].fd != -1)
            {
                continue;
            }

            s = sae_socket(listens[i].family, listens[i].type, listens[i].protocol, listens[i].flags);
            if (s == -1)
            {
                sae_log(LERROR, "sae_open_listening_sockets->sae_socket %s failed", listens[i].addr_text);
                return sae_false;
            }

#if (HAVE_WIN32)
            /*
             * Winsock assignes a socket number divisible by 4
             * so to find a connection we divide a socket number by 4
             */
            if (s % 4)
            {
                sae_log(LERROR, "created socket %d, because s % 4 != 0, Winsock assignes a socket number divisible by 4", s);
                return sae_false;
            }
#endif
            /*set reuse addr*/
            if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const sae_void_t *)&reuseaddr, sizeof(sae_int_t)) == -1)
            {
                sae_log(LERROR, "setsockopt(SO_REUSEADDR) %s failed, sock %d", listens[i].addr_text, s);
                sae_socket_close(s);
                return sae_false;
            }

            if (!(cycle->event_flag & (0x01)))
            {
                if (!sae_socket_nonblock(s))
                {
                    sae_log(LERROR, "sae_open_listening_sockets->sae_nonblock %s failed", listens[i].addr_text);
                    sae_socket_close(s);
                    return sae_false;
                }
            }

            if (bind(s, listens[i].sock_addr, listens[i].sock_len) == -1)
            {
                sae_log(LERROR, "bind() to %s failed", listens[i].addr_text);

                if (!sae_socket_close(s))
                {
                    sae_log(LERROR, "sae_open_listening_sockets->sae_socket_close %s failed", listens[i].addr_text);
                }

                failed = 1;
                
                continue;
            }

            if (listen(s, listens[i].back_log) == -1)
            {
                sae_log(LERROR, "listen() to %s failed", listens[i].addr_text);
                return sae_false;
            }

            /* TODO: deferred accept */
            listens[i].fd = s;
        }

        if (!failed)
        {
            break;
        }

        /* TODO: delay configurable */
        sae_log(LOTHER, "try again to bind() after 500ms");
        sae_msleep(500);
    }

    if (failed)
    {
        sae_log(LERROR, "still can not bind()");
        return sae_false;
    }

    return sae_true;
}

sae_void_t sae_close_listening_sockets(sae_cycle_core_t *cycle)
{
    sae_uint_t i = 0;
    sae_connection_listening_t *listens = sae_null;

    if (cycle->event_flag & (0x02))
    {
        return;
    }

    listens = cycle->listens->elts;
    for (i = 0; i < cycle->listens->nelts; i++)
    {
        /*todo event*/
        if (sae_socket_close(listens[i].fd) == -1)
        {
            sae_log(LERROR, " %s failed", listens[i].addr_text);
        }
        
        listens[i].fd = (sae_socket_fd_t)-1;
    }
}


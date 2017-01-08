
/*
 * Copyright (C) hanchong liu
 */


#include "sae_connection.h"

sae_connection_listening_t *sae_listening_socket_inet_stream(sae_connection_listening_t *listen,
                                                             sae_in_addr_t addr, sae_in_port_t port, sae_bool_t nonblock)
{
    sae_size_t len = 0;
    struct sockaddr_in *addr_in = sae_null;
    
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
    
    listen->nonblock = nonblock;

    return listen;
}

sae_bool_t sae_listening_socket_open(sae_connection_listening_t *listen)
{
    sae_uint_t reuse_addr = 1;
    sae_socket_fd_t s = (sae_socket_fd_t)-1;
    
    if (listen->ignore || listen->fd != -1)
    {
        return sae_true;
    }
    
    s = sae_socket(listen->family, listen->type, listen->protocol, listen->flags);
    if (s == -1)
    {
        sae_log(LERROR, "sae_open_listening_sockets->sae_socket %s failed", listen->addr_text);
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
    if (!sae_socket_opt(s, SOL_SOCKET, SO_REUSEADDR, (sae_void_t *)&reuse_addr, sizeof(sae_int_t)))
    {
        sae_log(LERROR, "setsockopt(SO_REUSEADDR) %s failed, sock %d", listen->addr_text, s);
        sae_socket_close(s);
        return sae_false;
    }
    
    if (listen->nonblock)
    {
        if (!sae_socket_nonblock(s))
        {
            sae_log(LERROR, "sae_open_listening_sockets->sae_nonblock %s failed", listen->addr_text);
            sae_socket_close(s);
            return sae_false;
        }
    }
    
    if (!sae_socket_bind(s, listen->sock_addr, listen->sock_len))
    {
        sae_log(LERROR, "bind() to %s failed", listen->addr_text);
        sae_socket_close(s);
        return sae_false;
    }
    
    if (!sae_socket_listen(s, listen->back_log))
    {
        sae_log(LERROR, "listen() to %s failed", listen->addr_text);
        return sae_false;
    }
    
    listen->fd = s;

    return sae_true;
}

sae_void_t sae_listening_socket_close(sae_connection_listening_t *listen)
{
    if (!sae_socket_close(listen->fd))
    {
        sae_log(LERROR, " %s failed", listen->addr_text);
    }
    
    listen->fd = (sae_socket_fd_t)-1;
}


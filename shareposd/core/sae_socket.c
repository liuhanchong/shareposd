
/*
 * Copyright (C) hanchong liu
 */


#include "sae_config.h"
#include "sae_core.h"

sae_socket_fd_t sae_socket(sae_int_t domain, sae_int_t type,
                                   sae_int_t protocol, sae_int_t flag)
{
#if (HAVE_WIN32)
    WSADATA wsa_data = {0};
    sae_connection_socket_t s = -1;
    
    /* Initialize Winsock */
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        return -1;
    }
    
    /*init socket*/
    s = WSASocket(domain, type, protocol, sae_null, 0, flag);
    if (s == INVALID_SOCKET)
    {
        WSACleanup();
        return -1;
    }
    
    return s;
#else
    return socket(domain, type, protocol);
#endif
}

sae_bool_t sae_socket_nonblock(sae_socket_fd_t fd)
{
#if (HAVE_WIN32)
    ULONG nonblock = 1;
    return (ioctlsocket(fd, FIONBIO, &nonblock) == SOCKET_ERROR) ? sae_false : sae_true;
#else
    return (fcntl(fd, F_SETFL, O_NONBLOCK) == -1 ? sae_false : sae_true);
#endif
}

sae_size_t sae_socket_recv(sae_socket_fd_t fd, sae_void_t *buf,
                           sae_size_t size, sae_int_t flag)
{
#if (HAVE_WIN32)
#error sae_socket_recv
#else
    return recv(fd, buf, size, flag);
#endif
}

sae_size_t sae_socket_send(sae_socket_fd_t fd, sae_void_t *buf,
                           sae_size_t size, sae_int_t flag)
{
#if (HAVE_WIN32)
#error sae_socket_send
#else
    return send(fd, buf, size, flag);
#endif
}

sae_bool_t sae_socket_close(sae_socket_fd_t fd)
{
#if (HAVE_WIN32)
    if (closesocket(fd) == SOCKET_ERROR)
    {
        sae_log(LERROR, "sae_socket_close->closesocket %d failed", fd);
        WSACleanup();
        return sae_false;
    }
    
    WSACleanup();
    
    return sae_true;
#else
    return (close(fd) == -1 ? sae_false : sae_true);
#endif
}

sae_bool_t sae_socket_pair(sae_socket_fd_t *sock_pair)
{
#if (HAVE_WIN32)
#error sae_socket_pair
#else
    return (socketpair(AF_UNIX, SOCK_STREAM, 0, sock_pair) == -1) ? sae_false : sae_true;
#endif
}

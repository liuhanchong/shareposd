
/*
 * Copyright (C) hanchong liu
 */


#include "sae_socket.h"

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
    s = WSASocket(domain, type, protocol, sae_null, 0, WSA_FLAG_OVERLAPPED);
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

sae_ssize_t sae_socket_recv(sae_socket_fd_t fd, sae_void_t *buf,
                           sae_size_t size, sae_int_t flag)
{
#if (HAVE_WIN32)
#error sae_socket_recv
    return recv(fd, (sae_char_t *)buf, (sae_int_t)size, flag);
#else
    return recv(fd, buf, size, flag);
#endif
}

sae_ssize_t sae_socket_send(sae_socket_fd_t fd, sae_void_t *buf,
                           sae_size_t size, sae_int_t flag)
{
#if (HAVE_WIN32)
#error sae_socket_send
    return send(fd, (sae_char_t *)buf, (sae_int_t)size, flag);
#else
    return send(fd, buf, size, flag);
#endif
}

sae_bool_t sae_socket_bind(sae_socket_fd_t fd, struct sockaddr *sock_addr, socklen_t sock_len)
{
    return (bind(fd, sock_addr, sock_len) != -1);
}

sae_bool_t sae_socket_listen(sae_socket_fd_t fd, sae_int_t back_log)
{
    return (listen(fd, back_log) != -1);
}

sae_bool_t sae_socket_connect(sae_socket_fd_t fd, struct sockaddr *sock_addr, socklen_t sock_len)
{
    return connect(fd, sock_addr, sock_len);
}

sae_socket_fd_t sae_socket_accept(sae_socket_fd_t fd, struct sockaddr *sock_addr, socklen_t sock_len)
{
    return accept(fd, sock_addr, &sock_len);
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
    /*create server*/
    sae_socket_fd_t server = sae_socket(AF_INET, SOCK_STREAM, 0, WSA_FLAG_OVERLAPPED);
    if (server == -1)
    {
        return sae_false;
    }
    
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = 0;
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t sock_len = sae_size_t(sock_addr);
    if (!sae_socket_bind(err_server, &sock_addr, sock_len))
    {
        sae_socket_close(server);
        return sae_false;
    }
    
    if (!sae_socket_listen(err_server, 1))
    {
        sae_socket_close(server);
        return sae_false;
    }
    
    /*create client*/
    sae_socket_fd_t client = sae_socket(AF_INET, SOCK_STREAM, 0, WSA_FLAG_OVERLAPPED);
    if (client == -1)
    {
        sae_socket_close(server);
        return sae_false;
    }
    
    sock_len = sae_size_t(sock_addr);
    if (getsockname(server, &sock_addr, &sock_len) == -1)
    {
        sae_socket_close(client);
        sae_socket_close(server);
        return sae_false;
    }
    
    if (sock_len != sae_size_t(sock_addr))
    {
        sae_socket_close(client);
        sae_socket_close(server);
        return sae_false;
    }
    
    if (!sae_socket_connect(client, sock_addr, sock_len))
    {
        sae_socket_close(client);
        sae_socket_close(server);
        return sae_false;
    }
    
    /*accept client*/
    accept_client = sae_socket_accept(server, &sock_addr, &sock_len);
    if (accept_client == -1)
    {
        sae_socket_close(client);
        sae_socket_close(server);
        return sae_false;
    }
    sae_socket_close(server);
    
    sock_pair[0] = client;
    sock_pair[1] = accept_client;
    
    return sae_true;
#else
    return (socketpair(AF_UNIX, SOCK_STREAM, 0, sock_pair) == -1) ? sae_false : sae_true;
#endif
}

sae_char_t *sae_socket_addr(struct sockaddr_in *sock_addr, socklen_t sock_len)
{
    static sae_char_t buf[25];
    
    sprintf(buf, "%s:%d", inet_ntoa(sock_addr->sin_addr), ntohs(sock_addr->sin_port));
    
    return buf;
}

sae_bool_t sae_socket_opt(sae_socket_fd_t fd, sae_int_t level, sae_int_t opt, sae_void_t *opt_val, socklen_t opt_len)
{
#if (HAVE_WIN32)
    return (setsockopt(fd, level, opt, opt_val, opt_len) == 0) ? sae_true : sae_false;
#else
    return (setsockopt(fd, level, opt, opt_val, opt_len) == 0) ? sae_true : sae_false;
#endif
}

sae_bool_t sae_socket_aton(const sae_char_t *ip, struct in_addr *addr)
{
#if (HAVE_WIN32)
    return (inet_aton(ip, addr) == 0) ? sae_false : sae_true;
#else
    return (inet_aton(ip, addr) == 0) ? sae_false : sae_true;
#endif
}

sae_uint16_t sae_socket_htons(sae_uint16_t port)
{
#if (HAVE_WIN32)
    return htons(port);
#else
    return htons(port);
#endif
}


/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_SOCKET_H_INCLUDED_
#define _SAE_SOCKET_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

sae_socket_fd_t sae_socket(sae_int_t domain,
                                   sae_int_t type, sae_int_t protocol, sae_int_t flag);

sae_bool_t sae_socket_nonblock(sae_socket_fd_t fd);

sae_ssize_t sae_socket_recv(sae_socket_fd_t fd, sae_void_t *buf,
                           sae_size_t size, sae_int_t flag);

sae_ssize_t sae_socket_send(sae_socket_fd_t fd, sae_void_t *buf,
                           sae_size_t size, sae_int_t flag);

sae_bool_t sae_socket_bind(sae_socket_fd_t fd, struct sockaddr *sock_addr, socklen_t sock_len);

sae_bool_t sae_socket_listen(sae_socket_fd_t fd, sae_int_t back_log);

sae_bool_t sae_socket_connect(sae_socket_fd_t fd, struct sockaddr *sock_addr, socklen_t sock_len);

sae_socket_fd_t sae_socket_accept(sae_socket_fd_t fd, struct sockaddr *sock_addr, socklen_t sock_len);

sae_bool_t sae_socket_close(sae_socket_fd_t fd);

sae_bool_t sae_socket_pair(sae_socket_fd_t *sock_pair);

/*note: the fun is single thread*/
sae_char_t *sae_socket_addr(struct sockaddr_in *sock_addr, socklen_t sock_len);

#endif /* _SAE_SOCKET_H_INCLUDED_ */

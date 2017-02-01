
/*
 * Copyright (C) hanchong liu
 */


#ifndef _LINUX_BASE_H_INCLUDED_
#define _LINUX_BASE_H_INCLUDED_

#include "../../core/sae_type.h"

sae_bool_t sae_res_fd_max_set(sae_uint64_t n);

sae_uint64_t sae_res_fd_max_get();

sae_bool_t sae_res_core_size_set(sae_uint32_t s);

sae_uint32_t sae_res_core_size_get();

sae_ushort_t sae_res_cpu_num_get();

sae_bool_t sae_res_signal_set(sae_int_t sig, sae_signal_t *news, sae_signal_t *olds);

sae_pid_t sae_res_pid_get();

sae_pid_t sae_res_process_fork();

sae_pid_t sae_res_session_set();

sae_socket_fd_t sae_socket(sae_int_t domain, sae_int_t type,
                           sae_int_t protocol, sae_int_t flag);

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

sae_char_t *sae_socket_addr(struct sockaddr_in *sock_addr, socklen_t sock_len);

sae_bool_t sae_socket_opt(sae_socket_fd_t fd, sae_int_t level, sae_int_t opt, sae_void_t *opt_val, socklen_t opt_len);

sae_bool_t sae_socket_aton(const sae_char_t *ip, struct in_addr *addr);

sae_uint16_t sae_socket_htons(sae_uint16_t port);

sae_int_t sae_space(sae_int_t c);

#define sae_strlen(s) strlen((sae_char_t *)s)
#define sae_memzero(buf, n) memset(buf, 0, n)
#define sae_memcpy(dst, src, n) memcpy(dst, src, n)
#define sae_strcmp(s1, s2)  strcmp((sae_cchar_t *)s1, (sae_cchar_t *)s2)
#define sae_strrchr(s, c)  strrchr((sae_cchar_t *)s, (sae_int_t)c)
#define sae_strchr(s, c)  strchr((sae_cchar_t *)s, (sae_int_t)c)
#define sae_isspace(c)  sae_space((sae_int_t)c)
#define sae_strncpy(dst, src, n) strncpy((sae_char_t *)dst, (sae_cchar_t *)src, (sae_size_t)n)
#define sae_strncmp(s1, s2, n)  strncmp((sae_cchar_t *)s1, (sae_cchar_t *)s2, (sae_size_t)n)
#define sae_strncmp_parse(s1, s2, n)  strncmp((sae_cchar_t *)s1, (sae_parse_char_t *)s2, (sae_size_t)n)
#define sae_snprintf(str, len, format, ...) snprintf(str, len, format, __VA_ARGS__)
#define sae_vsnprintf(str, len, format, arg_list) vsnprintf(str, len, format, arg_list)

#define sae_cp_str(dst, src, n) \
sae_memcpy(dst, src, len); \
dst[len] = sae_str_end;

sae_ulong_t sae_str_hash(sae_char_t *str);

sae_void_t sae_str_trim(sae_char_t *str);

sae_char_t *sae_int_to_str(sae_int_t v);

#endif /* _LINUX_BASE_H_INCLUDED_ */

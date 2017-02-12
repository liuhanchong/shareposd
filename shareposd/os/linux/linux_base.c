
/*
 * Copyright (C) hanchong liu
 */


#include "linux_base.h"

sae_bool_t sae_res_fd_max_set(sae_uint64_t n)
{
    struct rlimit rlt;
    if (getrlimit(RLIMIT_NOFILE, &rlt) == 0)
    {
        rlt.rlim_cur = (n >= rlt.rlim_max) ? rlt.rlim_max : n;
        if (setrlimit(RLIMIT_NOFILE, &rlt) == 0)
        {
            return sae_true;
        }
    }
    
    return sae_false;
}

sae_uint64_t sae_res_fd_max_get()
{
    struct rlimit rlt;
    return (getrlimit(RLIMIT_NOFILE, &rlt) == 0) ? (sae_uint64_t)rlt.rlim_cur : 256;
}

sae_bool_t sae_res_core_size_set(sae_uint32_t s)
{
    struct rlimit rlt;
    if (getrlimit(RLIMIT_CORE, &rlt) == 0)
    {
        rlt.rlim_cur = (s >= rlt.rlim_max) ? rlt.rlim_max : s;
        if (setrlimit(RLIMIT_CORE, &rlt) == 0)
        {
            return sae_true;
        }
    }
    
    return sae_false;
}

sae_uint32_t sae_res_core_size_get()
{
    struct rlimit rlt;
    return (getrlimit(RLIMIT_CORE, &rlt) == 0) ? (sae_uint32_t)rlt.rlim_cur : 0;
}

sae_ushort_t sae_res_cpu_num_get()
{
    return (sae_ushort_t)sysconf(_SC_NPROCESSORS_ONLN);
}

sae_bool_t sae_res_signal_set(sae_int_t sig, sae_signal_t *news, sae_signal_t *olds)
{
    return (sigaction(sig, news, olds) == -1) ? sae_false : sae_true;
}

sae_pid_t sae_res_pid_get()
{
    return getpid();
}

sae_pid_t sae_res_process_fork()
{
    return fork();
}

sae_pid_t sae_res_session_set()
{
    return setsid();
}

sae_socket_fd_t sae_socket(sae_int_t domain, sae_int_t type,
                           sae_int_t protocol, sae_int_t flag)
{
    return socket(domain, type, protocol);
}

sae_bool_t sae_socket_nonblock(sae_socket_fd_t fd)
{
    return (fcntl(fd, F_SETFL, O_NONBLOCK) == -1 ? sae_false : sae_true);
}

sae_ssize_t sae_socket_recv(sae_socket_fd_t fd, sae_void_t *buf,
                            sae_size_t size, sae_int_t flag)
{
    return recv(fd, buf, size, flag);
}

sae_ssize_t sae_socket_send(sae_socket_fd_t fd, sae_void_t *buf,
                            sae_size_t size, sae_int_t flag)
{
    return send(fd, buf, size, flag);
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
    return (close(fd) == -1 ? sae_false : sae_true);
}

sae_bool_t sae_socket_pair(sae_socket_fd_t *sock_pair)
{
    return (socketpair(AF_UNIX, SOCK_STREAM, 0, sock_pair) == -1) ? sae_false : sae_true;
}

sae_char_t *sae_socket_addr(struct sockaddr_in *sock_addr, socklen_t sock_len)
{
    static sae_char_t buf[25];
    
    sprintf(buf, "%s:%d", inet_ntoa(sock_addr->sin_addr), ntohs(sock_addr->sin_port));
    
    return buf;
}

sae_bool_t sae_socket_opt(sae_socket_fd_t fd, sae_int_t level, sae_int_t opt, sae_void_t *opt_val, socklen_t opt_len)
{
    return (setsockopt(fd, level, opt, opt_val, opt_len) == 0) ? sae_true : sae_false;
}

sae_bool_t sae_socket_aton(const sae_char_t *ip, struct in_addr *addr)
{
    return (inet_aton(ip, addr) == 0) ? sae_false : sae_true;
}

sae_uint16_t sae_socket_htons(sae_uint16_t port)
{
    return htons(port);
}

sae_int_t sae_space(sae_int_t c)
{
    return (c == ' '  || c == '\t' ||
            c == '\r' || c == '\n' ||
            c == '\v' || c == '\f') ? 1 : 0;
}

sae_ulong_t sae_str_hash(sae_char_t *str)
{
    sae_int_t i = 0;
    sae_ulong_t ret = 0;
    sae_int_t l = 0;
    sae_ushort_t *s = sae_null;
    
    if (str == sae_null)
    {
        return 0;
    }
    
    l = (sae_int_t)((sae_strlen(str) + 1) / 2);
    s = (sae_ushort_t *)str;
    for (i = 0; i < l; ++i)
    {
        ret ^= (s[i] << (i & 0x0f));
    }
    
    return ret;
}

sae_void_t sae_str_trim(sae_char_t *str)
{
    sae_size_t begin = 0;
    sae_size_t end = 0;
    sae_char_t *cp = sae_null;
    
    if ((end = sae_strlen(str)) < 1)
    {
        return;
    }
    
    /*alloc cp str*/
    cp = sae_malloc(end);
    
    /*cp str*/
    sae_memcpy(cp, str, end);
    
    while (sae_isspace(str[begin++]));
    begin--;
    
    while (sae_isspace(str[--end]));
    
    sae_strncpy(str, cp + begin, (end - begin + 1));
    
    sae_free(cp);
    
    str[(end - begin + 1)] = sae_str_end;
}

sae_char_t *sae_int_to_str(sae_int_t v)
{
    sae_size_t len = (v > 0) ? 1 : 2;
    sae_int_t t = (v > 0) ? v : -v;
    
    while ((++len) && (t = (t / 10)));
    
    sae_char_t *buf = sae_malloc(len);
    
    sae_snprintf(buf, len, "%d", v);
    
    return buf;
}

sae_void_t sae_str_cp(sae_char_t *dst, sae_cchar_t *src, sae_size_t n)
{
    sae_memcpy(dst, src, n);
    dst[n] = sae_str_end;
}


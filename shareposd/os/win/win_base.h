
/*
 * Copyright (C) hanchong liu
 */


#ifndef _WIN_BASE_H_INCLUDED_
#define _WIN_BASE_H_INCLUDED_

sae_bool_t sae_res_fd_max_set(sae_uint64_t n)
{
#error sae_res_fd_max_set
}

sae_uint64_t sae_res_fd_max_get()
{
#error sae_res_fd_max_get
}

sae_bool_t sae_res_core_size_set(sae_uint32_t s)
{
#error sae_res_core_size_set
}

sae_uint32_t sae_res_core_size_get()
{
#error sae_res_core_size_get
}

sae_ushort_t sae_res_cpu_num_get()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors;
}

sae_bool_t sae_res_signal_set(sae_int_t sig, sae_signal_t *news, sae_signal_t *olds)
{
#error sae_res_signal_set
    return ((*olds = signal(sig, *news)) == SIG_ERR) ? sae_false : sae_true;
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
}

sae_bool_t sae_socket_nonblock(sae_socket_fd_t fd)
{
    ULONG nonblock = 1;
    return (ioctlsocket(fd, FIONBIO, &nonblock) == SOCKET_ERROR) ? sae_false : sae_true;
}

sae_ssize_t sae_socket_recv(sae_socket_fd_t fd, sae_void_t *buf,
                            sae_size_t size, sae_int_t flag)
{
#error sae_socket_recv
    return recv(fd, (sae_char_t *)buf, (sae_int_t)size, flag);
}

sae_ssize_t sae_socket_send(sae_socket_fd_t fd, sae_void_t *buf,
                            sae_size_t size, sae_int_t flag)
{
#error sae_socket_send
    return send(fd, (sae_char_t *)buf, (sae_int_t)size, flag);
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
    if (closesocket(fd) == SOCKET_ERROR)
    {
        sae_log(LERROR, "sae_socket_close->closesocket %d failed", fd);
        WSACleanup();
        return sae_false;
    }
    
    WSACleanup();
    
    return sae_true;
}

sae_bool_t sae_socket_pair(sae_socket_fd_t *sock_pair)
{
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

#define sae_strlen(s) strlen((sae_char_t *)s)
#define sae_memzero(buf, n) memset(buf, 0, n)
#define sae_memcpy(dst, src, n) memcpy(dst, src, n)
#define sae_strcmp(s1, s2)  strcmp((sae_cchar_t *)s1, (sae_cchar_t *)s2)
#define sae_strrchr(s, c)  strrchr((sae_cchar_t *)s, (sae_int_t)c)
#define sae_strchr(s, c)  strchr((sae_cchar_t *)s, (sae_int_t)c)
#define sae_isspace(c)  sae_space((sae_int_t)c)
#define sae_strncpy(dst, src, n) strncpy((sae_char_t *)dst, (sae_cchar_t *)src, (sae_size_t)n)
#define sae_strncmp(s1, s2, n)  strncmp((sae_cchar_t *)s1, (sae_cchar_t *)s2, (sae_size_t)n)
sae_int_t sae_strncmp_parse(sae_cchar_t *s1, sae_parse_char_t *s2, sae_size_t n);
#define sae_snprintf(str, len, format, ...) sprintf_s(str, len, format, __VA_ARGS__)
#define sae_vsnprintf(str, len, format, arg_list) vsnprintf_s(str, len, _TRUNCATE, format, arg_list)

#define sae_cp_str(dst, src, n) \
sae_memcpy(dst, src, len); \
dst[len] = sae_str_end;

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
    cp = sae_alloc(end);
    
    /*cp str*/
    sae_memcpy(cp, str, end);
    
    while (sae_isspace(str[begin++]));
    begin--;
    
    while (sae_isspace(str[--end]));
    
    sae_strncpy(str, cp + begin, (end - begin + 1));
    
    sae_alloc_free(cp);
    
    str[(end - begin + 1)] = sae_str_end;
}

WCHAR *sae_char_to_utf8(sae_cchar_t *str)
{
    sae_uint_t n = 0;
    WCHAR *utf_str = sae_null;
    
    /*get tran Unicode len*/
    n = MultiByteToWideChar(CP_UTF8, 0, str, -1, sae_null, 0);
    
    /*alloc utf*/
    utf_str = sae_alloc_n(sae_sizeof(WCHAR), n + 1);
    
    /*tran to utf8*/
    n = MultiByteToWideChar(CP_UTF8, 0, str, -1, utf_str, n);
    
    if(n == 0)
    {
        sae_alloc_free(utf_str);
        return sae_null;
    }
    
    return utf_str;
}

sae_int_t sae_strncmp_parse(sae_cchar_t *s1, sae_parse_char_t *s2, sae_size_t n)
{
    sae_parse_char_t *ws1 = sae_char_to_utf8(s1);
    
    sae_int_t ret = wcscmp(ws1, s2);
    
    sae_alloc_free(ws1);
    
    return ret;
}

sae_char_t *sae_int_to_str(sae_int_t v)
{
    sae_size_t len = (v > 0) ? 1 : 2;
    sae_int_t t = (v > 0) ? v : -v;
    
    while ((++len) && (t = (t / 10)));
    
    sae_char_t *buf = sae_alloc(len);
    
    sae_snprintf(buf, len, "%d", v);
    
    return buf;
}

#endif /* _WIN_BASE_H_INCLUDED_ */

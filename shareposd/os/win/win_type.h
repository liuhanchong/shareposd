
/*
 * Copyright (C) hanchong liu
 */


#ifndef _WIN_TYPE_H_INCLUDED_
#define _WIN_TYPE_H_INCLUDED_

#include <Winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <signal.h>
#pragma comment(lib,"ws2_32.lib")

typedef HANDLE sae_file_fd_t;
typedef WCHAR sae_parse_char_t;
typedef SOCKET sae_socket_fd_t;
typedef ULONG sae_in_addr_t;
typedef u_short sae_in_port_t;
typedef struct in_addr sae_sin_addr_t;
typedef pid_t sae_pid_t;
#error windows signal
typedef sighandler_t sae_signal_t;
typedef sae_lock_t;
typedef HANDLE sae_thread_id_t;

#define sae_path_seg ('\\')
#define SAE_SIGNAL_NUM 25

#endif /* _WIN_TYPE_H_INCLUDED_ */

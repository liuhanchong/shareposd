
/*
 * Copyright (C) hanchong liu
 */


#ifndef _LINUX_TYPE_H_INCLUDED_
#define _LINUX_TYPE_H_INCLUDED_

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

typedef sae_int_t sae_file_fd_t;
typedef sae_char_t sae_parse_char_t;
typedef in_addr_t sae_in_addr_t;
typedef in_port_t sae_in_port_t;
typedef sae_int_t sae_socket_fd_t;
typedef struct in_addr sae_sin_addr_t;
typedef struct sigaction sae_signal_t;
typedef pid_t sae_pid_t;
typedef pthread_mutex_t sae_lock_t;
typedef pthread_t sae_thread_id_t;

#define sae_path_seg ('/')
#define SAE_SIGNAL_NUM 65

#endif /* _LINUX_TYPE_H_INCLUDED_ */

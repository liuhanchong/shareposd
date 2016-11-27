
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_CONFIG_H_INCLUDED_
#define _SAE_CONFIG_H_INCLUDED_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/types.h>
#include <stddef.h>

#if (WIN32)
#define HAVE_WIN32 1
#endif

#if (HAVE_WIN32)
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include<dirent.h>
#endif

/*sys base type*/
typedef int sae_int_t;
typedef const char sae_cchar_t;
typedef const int sae_cint_t;
typedef long sae_long_t;
typedef char sae_char_t;
typedef void sae_void_t;
typedef unsigned int sae_uint_t;
typedef unsigned long sae_ulong_t;
typedef unsigned short sae_ushort_t;
typedef short sae_short_t;
typedef double sae_double_t;
typedef float sae_float_t;
typedef uint32_t sae_uint32_t;
typedef uint64_t sae_uint64_t;
typedef int32_t sae_int32_t;

/*sys define type*/
typedef size_t sae_size_t;
typedef off_t sae_off_t;

/*array*/
typedef struct sae_array_s sae_array_t;

/*inline*/
#ifdef __cplusplus
#define sae_inline inline
#else
#define sae_inline
#endif

/*mem alloc and free*/
#define sae_malloc malloc
#define sae_free free

/*size cal*/
#define sae_sizeof sizeof

/*null*/
#define sae_null NULL

/*mem unit*/
#define sae_kb (1024)
#define sae_mb ((1024) * sae_kb)

/*bool true and false*/
typedef sae_int_t sae_bool_t;
#define sae_true 1
#define sae_false 0

/*str rel*/
#define sae_str_end ('\0') /*str end flag*/

#if (HAVE_WIN32) /*path seq*/
#define sae_path_seg ('\\')
#else
#define sae_path_seg ('/')
#endif

typedef struct sae_file_s sae_file_t;

/*fd type*/
#if (HAVE_WIN32)
typedef HANDLE sae_file_fd_t;
#else
typedef sae_int_t sae_file_fd_t;
#endif

/*file op*/
typedef sae_int_t sae_file_op_t;
#define SAE_FILE_READ (0x01)
#define SAE_FILE_WRITE (0x02)
#define SAE_FILE_AUTO_CRE (0x08)

/*log config*/
#define SAE_LOG_TYPE_COUNT (LDUMP + 1) /*LDUMP the sae_log_type_e enum last*/
#define SAE_LOG_SIZE (sae_kb / 2) /*every log info max len*/
#define SAE_LOG_PATH_LEN (sae_kb / 4) /*log path max len*/
typedef enum sae_log_type_e sae_log_type_t;
typedef struct sae_log_s sae_log_t;

/*list*/
typedef struct sae_list_node_s sae_list_node_t;
typedef struct sae_list_s sae_list_t;

/*table*/
typedef struct sae_table_elt_s sae_table_elt_t;
typedef struct sae_table_s sae_table_t;

/*heap*/
typedef struct sae_heap_elt_s sae_heap_elt_t;
typedef struct sae_heap_s sae_heap_t;
typedef sae_short_t (*sae_heap_elt_cmp)(sae_heap_elt_t *src, sae_heap_elt_t *dst);

/*thread*/
#if (HAVE_WIN32)
typedef HANDLE sae_thread_id_t;
#else
#include <pthread.h>
typedef pthread_t sae_thread_id_t;
#endif
typedef struct sae_thread_s sae_thread_t;
typedef sae_void_t *(*sae_thread_task)(sae_void_t *);

/*parse conf*/
typedef struct sae_parse_s sae_parse_t;
#define SAE_PARSE_ROW_MAX_LEN (sae_kb / 4)
#if (HAVE_WIN32)
typedef WCHAR sae_parse_char_t;
#else
typedef sae_char_t sae_parse_char_t;
#endif

/*connection*/
#if (HAVE_WIN32)
typedef SOCKET sae_socket_fd_t;
typedef ULONG sae_in_addr_t;
typedef u_short sae_in_port_t;
#else
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
typedef in_addr_t sae_in_addr_t;
typedef in_port_t sae_in_port_t;
typedef sae_int_t sae_socket_fd_t;
#endif
typedef struct sae_connection_listening_s sae_connection_listening_t;
typedef struct sae_connection_s sae_connection_t;

/*cycle*/
typedef struct sae_cycle_core_s sae_cycle_core_t;
typedef struct sae_cycle_conf_s sae_cycle_conf_t;

/*signal*/
#if (HAVE_WIN32)
#error windows signal
#include <signal.h>
typedef sighandler_t sae_signal_t;
#define SAE_SIGNAL_NUM 25
#else
#include <signal.h>
typedef struct sigaction sae_signal_t;
#define SAE_SIGNAL_NUM 65
#endif

#include <assert.h>

/*depend auto config compile generate config*/
#include "config.h"

#if (HAVE_WIN32)
typedef pid_t sae_pid_t;
#else
typedef pid_t sae_pid_t;
#endif

#endif /* _SAE_CONFIG_H_INCLUDED_ */

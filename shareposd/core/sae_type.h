
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_TYPE_H_INCLUDED_
#define _SAE_TYPE_H_INCLUDED_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/types.h>
#include <stddef.h>
#include <assert.h>

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
typedef uint16_t sae_uint16_t;
typedef sae_int_t sae_bool_t;
typedef size_t sae_size_t;
typedef off_t sae_off_t;
typedef ssize_t sae_ssize_t;

/*self struct type*/
typedef struct sae_array_s sae_array_t;
typedef struct sae_file_s sae_file_t;
typedef sae_int_t sae_file_op_t;
typedef enum sae_log_type_e sae_log_type_t;
typedef struct sae_log_s sae_log_t;
typedef struct sae_list_node_s sae_list_node_t;
typedef struct sae_list_s sae_list_t;
typedef struct sae_table_elt_s sae_table_elt_t;
typedef struct sae_table_s sae_table_t;
typedef struct sae_heap_elt_s sae_heap_elt_t;
typedef struct sae_heap_s sae_heap_t;
typedef struct sae_parse_s sae_parse_t;
typedef struct sae_connection_listening_s sae_connection_listening_t;
typedef struct sae_connection_s sae_connection_t;
typedef struct sae_cycle_core_s sae_cycle_core_t;
typedef struct sae_cycle_conf_s sae_cycle_conf_t;
typedef struct sae_buffer_s sae_buffer_t;
typedef struct sae_thread_s sae_thread_t;

/*self fun type*/
typedef sae_short_t (*sae_heap_elt_cmp)(sae_heap_elt_t *src, sae_heap_elt_t *dst);
typedef sae_void_t *(*sae_thread_task)(sae_void_t *);

#define sae_malloc malloc
#define sae_free free
#define sae_sizeof sizeof
#define sae_null NULL
#define sae_kb (1024)
#define sae_mb ((1024) * sae_kb)
#define sae_true 1
#define sae_false 0
#define sae_str_end ('\0') /*str end flag*/
#define SAE_FILE_READ (0x01)
#define SAE_FILE_WRITE (0x02)
#define SAE_FILE_AUTO_CRE (0x08)
#define SAE_LOG_TYPE_COUNT (LDUMP + 1) /*LDUMP the sae_log_type_e enum last*/
#define SAE_LOG_SIZE (sae_kb / 2) /*every log info max len*/
#define SAE_LOG_PATH_LEN (sae_kb / 4) /*log path max len*/
#define SAE_PARSE_ROW_MAX_LEN (sae_kb / 4)
#define SAE_BUFFER_MAX_LEN 65535
#define SAE_BUFFER_INIT_LEN 1024
#define SAE_BUFFER_LINE_FLAG "\n"

#ifdef __cplusplus
#define sae_inline inline
#else
#define sae_inline
#endif

#if (HAVE_WIN32)
#include "../os/win/win_type.h"
#else
#include "../os/linux/linux_type.h"
#endif

#endif /* _SAE_TYPE_H_INCLUDED_ */

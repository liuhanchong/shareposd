
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_EVENT_H_INCLUDED_
#define _SAE_EVENT_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

/*moudle load*/
#ifdef HAVE_SYS_EVENT_H
#include <sys/event.h>
#define HAVE_KQUEUE 1
#endif

#ifdef HAVE_SYS_EPOLL_H
#include <sys/epoll.h>
#define HAVE_EPOLL 1
#endif

#ifdef HAVE_SYS_POLL_H
#include <sys/poll.h>
#define HAVE_POLL 1
#endif

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#define HAVE_SELECT 1
#endif

typedef struct sae_event_s sae_event_t;
typedef struct sae_event_base_s sae_event_base_t;
typedef struct sae_event_top_s sae_event_top_t;
typedef sae_int_t sae_event_fd_t;
typedef sae_void_t *(*sae_event_call_back)(sae_void_t *, sae_void_t *);
typedef sae_bool_t (*sae_event_base_handle_call_back)(sae_event_t *event);

struct sae_event_s
{
    sae_event_fd_t event_fd;/*socket signal timers*/
    sae_int_t event_flag;/*read write signal timer*/
    sae_event_base_t *event_base;
    sae_int_t event_state;/*event state insert active*/
    sae_signal_t event_signal_old;/*signal event save old signal*/
    sae_char_t *event_fd_str;/*str fd value*/
    sae_event_call_back event_call;
    sae_void_t *event_call_arg;
    struct timeval event_timer_end;/*timer end*/
    struct timeval event_timer_wait;/*wait time*/
    sae_event_t *event_next;
};

struct sae_event_base_s
{
    /*read write event*/
    sae_table_t *event_table_socket;
    
    /*timers event*/
    sae_heap_t *event_heap_timer;/*timer event heap*/
    
    /*signal event*/
    sae_list_t *event_list_signal;/*signal event list*/
    sae_int_t event_signal_array_active[SAE_SIGNAL_NUM];/*active signal list*/
    sae_bool_t event_signal_state;/*flase-no trigger true-trigger*/
    sae_socket_fd_t event_signal_sock_pair[2];/*trigger signal*/
    sae_event_t *event_signal_sock_pair_read;/*save socket pair read event*/
    sae_event_t *event_signal_call_stop;/*save stop dispatch event*/
    
    /*active event*/
    sae_list_t *event_list_active;/*active list*/
    
    /*event base*/
    sae_bool_t event_base_work;/*is work*/
    sae_uint_t event_base_fd_max;/*max fd num*/
    struct timeval event_base_timer_wait;/*def outtime*/
    sae_event_top_t *event_base_module_select;/*select module*/
    sae_void_t *event_base_module_select_instance;/*select module instance*/
    sae_event_base_handle_call_back event_base_handle_call;/*event handle call*/
};

struct sae_event_top_s
{
    sae_cchar_t *name;/*module name*/
    sae_void_t *(*create)(sae_void_t *);
    sae_bool_t (*add)(sae_event_t *, sae_void_t *);
    sae_bool_t (*del)(sae_event_t *, sae_void_t *);
    sae_bool_t (*dispatch)(sae_event_base_t *, struct timeval *, sae_void_t *);
    sae_bool_t (*destroy)(sae_event_base_t *, sae_void_t *);
};

/*create event moudle*/
typedef sae_array_t sae_event_top_array_t;
sae_event_top_array_t *sae_event_top_create();
void sae_event_top_destroy(sae_event_top_array_t *array);

/*event flag*/
#define SAE_EVENT_TIMER 0x01 /*timer event*/
#define SAE_EVENT_READ 0x02    /*read event*/
#define SAE_EVENT_WRITE 0x04   /*write event*/
#define SAE_EVENT_SIGNAL 0x08  /*signal event*/
#define SAE_EVENT_PERSIST 0x10 /*persist event*/
#define SAE_EVENT_ET 0x20 /*edge trigger*/

/*event state*/
#define SAE_EVENT_STATE_INSERT 0x01 /*insert to kernel*/
#define SAE_EVENT_STATE_ACTIVE 0x02 /*active to kernel*/
#define SAE_EVENT_STATE_DELETE 0x04 /*del to kernel*/

/*set socket event*/
#define sae_event_socket_set(base, fd, flag, call, arg) \
                    sae_event_set(base, fd, flag, call, arg)

/*set signal event*/
#define sae_event_signal_set(base, sig, flag, call, arg) \
                    sae_event_set(base, sig, flag | SAE_EVENT_SIGNAL, call, arg)

/*set timers event*/
#define sae_event_timer_set(base, flag, call, arg) \
                    sae_event_set(base, -1, flag | SAE_EVENT_TIMER, call, arg)

/*add socket event*/
#define sae_event_socket_add(event) sae_event_add(event, sae_null)

/*add signal event*/
#define sae_event_signal_add(event) sae_event_add(event, sae_null)

/*add timer event*/
#define sae_event_timer_add(event, time) sae_event_add(event, time)

sae_event_t *sae_event_set(sae_event_base_t *base, sae_event_fd_t fd,
                           sae_int_t flag, sae_event_call_back call, sae_void_t *arg);

sae_bool_t sae_event_add(sae_event_t *event, struct timeval *tv);

sae_bool_t sae_event_del(sae_event_t *event);

sae_bool_t sae_event_active(sae_event_t *event);

sae_void_t sae_event_free(sae_event_t *event);

sae_event_base_t *sae_event_base_create(sae_event_top_t *module, sae_event_base_handle_call_back call);

sae_void_t sae_event_base_destroy(sae_event_base_t *base);

sae_bool_t sae_event_base_dispatch(sae_event_base_t *base);

#endif /* _SAE_EVENT_H_INCLUDED_ */

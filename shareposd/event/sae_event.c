
/*
 * Copyright (C) hanchong liu
 */


#include "sae_event.h"

/*global reactor save signal event*/
static sae_event_base_t *event_base_global = sae_null;

static sae_void_t *sae_event_base_call_stop(sae_event_t *event, sae_void_t *arg)
{
    ((sae_event_base_t *)arg)->event_base_work = sae_false;
    
    sae_log(LDEBUG, "sae_event_base_call_stop");
    
    return sae_null;
}

static sae_short_t sae_event_timer_call_cmp(sae_heap_elt_t *src, sae_heap_elt_t *dst)
{
    return sae_times_timeval_cmp(&(((sae_event_t *)src)->event_timer_end),
                                 &(((sae_event_t *)dst)->event_timer_end));
}

#if (HAVE_WIN32)
#error sae_event_signal_event_call_set
static sae_void_t sae_event_signal_event_call_set(sae_int_t sig)
#else
static sae_void_t sae_event_signal_event_call_set(sae_int_t sig, siginfo_t *siginfo, sae_void_t *arg)
#endif
{
    if (!event_base_global)
    {
        sae_log(LERROR, "%s->%s null", "sae_event_signal_event_call_set", "event_base_global");
        return;
    }
    
    if (sig > SAE_SIGNAL_NUM || sig < 1)
    {
        sae_log(LERROR, "%s sig value is illeagal", "sae_event_signal_event_call_set");
        return;
    }
    
    event_base_global->event_signal_array_active[sig] = sig;
    event_base_global->event_signal_state = sae_true;
    
    sae_socket_send(event_base_global->event_signal_sock_pair[0], "a", 1, 0);
}

static sae_void_t *sae_event_signal_call_reset(sae_event_t *event, sae_void_t *arg)
{
    sae_socket_fd_t fd = event->event_fd;
    sae_socket_fd_t socket_pair_fd_read = ((sae_event_t *)event)->event_base->event_signal_sock_pair[1];
    
    if (fd == socket_pair_fd_read)
    {
        sae_char_t buf[2];
        sae_socket_recv(fd, buf, sae_sizeof(buf), 0);
    }
    
    return sae_null;
}

static struct timeval sae_event_timer_min_get(sae_event_base_t *base, struct timeval *def_timer)
{
    sae_event_t *event = sae_null;
    struct timeval min_timer;
    
    if (!(event = sae_heap_value_min_get(base->event_heap_timer)))
    {
        return *def_timer;
    }
    
    min_timer = event->event_timer_end;
    if ((min_timer.tv_sec -= time(sae_null)) < 0)
    {
        sae_memzero(&min_timer, sae_sizeof(struct timeval));
    }
    
    return (sae_times_timeval_cmp(&min_timer, def_timer) == -1) ? *def_timer : min_timer;
}

static sae_bool_t sae_event_signal_active_get(sae_event_base_t *base)
{
    sae_int_t i = 0;
    sae_int_t sig = 0;
    sae_event_t *event = sae_null;
    sae_list_node_t *node = sae_null;
    
    /*no active*/
    if (!base->event_signal_state)
    {
        return sae_true;
    }
    
    base->event_signal_state = sae_false;
    
    for (i = 1; i < SAE_SIGNAL_NUM; ++i)
    {
        /*is signal active*/
        if ((sig = base->event_signal_array_active[i]) == 0)
        {
            continue;
        }
        
        /*reset*/
        base->event_signal_array_active[i] = 0;
        
        /*add active list*/
        sae_list_for (base->event_list_signal, node)
        {
            event = node->data;
            if (sig == event->event_fd)
            {
                if (!(event->event_flag & SAE_EVENT_PERSIST))
                {
                    sae_event_del(event);
                }
                
                sae_event_active(event);
            }
        }
    }
    
    return sae_true;
}

static sae_bool_t sae_event_timer_active_get(sae_event_base_t *base)
{
    sae_uint_t i = 0;
    struct timeval tv = {time(sae_null), 0};
    sae_event_t *event = sae_null;
    
    for (i = 0; i < sae_heap_size_get(base->event_heap_timer); ++i)
    {
        event = sae_heap_value_get(base->event_heap_timer, i);
        
        if (sae_times_timeval_cmp(&tv, &event->event_timer_end) != -1)
        {
            if (event->event_flag & SAE_EVENT_PERSIST)
            {
                event->event_timer_end.tv_sec = time(sae_null) + event->event_timer_wait.tv_sec;
            }
            else
            {
                sae_event_del(event);
            }
            
            sae_event_active(event);
        }
    }
    
    return sae_true;
}

sae_event_t *sae_event_set(sae_event_base_t *base, sae_event_fd_t fd,
                           sae_int_t flag, sae_event_call_back call, sae_void_t *arg)
{
    sae_event_t *event = sae_null;
    
    if (!(event = sae_alloc(sae_sizeof(sae_event_t))))
    {
        return event;
    }
    
    event->event_base = base;
    event->event_fd = fd;
    event->event_flag = flag;
    event->event_state = 0;
    event->event_next = sae_null;
    event->event_call = call;
    event->event_call_arg = arg;
    event->event_fd_str = sae_int_to_str(fd);
    
    return event;
}

sae_bool_t sae_event_add(sae_event_t *event, struct timeval *tv)
{
    sae_signal_t sig;
    
    if (event->event_flag & SAE_EVENT_READ || event->event_flag & SAE_EVENT_WRITE)
    {
        /*find exist event*/
        sae_event_t *event_find = sae_table_value(event->event_base->event_table_socket, event->event_fd_str);
        if (event_find)
        {
            if (!sae_event_del(event_find))
            {
                sae_log(LERROR, "sae_event_add->sae_event_del exist event failed clientsock=%d", event_find->event_fd);
                return sae_false;
            }
        }
        
        /*add kernel*/
        if (!event->event_base->event_base_module_select->add(event, event->event_base->event_base_module_select_instance))
        {
            return sae_false;
        }
        
        /*add user level*/
        if (!sae_table_push(event->event_base->event_table_socket, event->event_fd_str, event))
        {
            return sae_false;
        }
    }
    else if (event->event_flag & SAE_EVENT_SIGNAL)
    {
        event_base_global = event->event_base;
        
#if (HAVE_WIN32)
#error sae_event_add
        sig = sae_event_signal_event_call_set;
#else
        sigemptyset(&sig.sa_mask);
        sig.sa_sigaction = sae_event_signal_event_call_set;
        sig.sa_flags = SA_SIGINFO | SA_RESTART;
#endif
        if (!sae_res_signal_set(event->event_fd, &sig, &event->event_signal_old))
        {
            return sae_false;
        }
        
        if (!(event->event_signal_list_node = sae_list_push(event->event_base->event_list_signal, event)))
        {
            return sae_false;
        }
    }
    else if (event->event_flag & SAE_EVENT_TIMER)
    {
        sae_memcpy(&event->event_timer_wait, tv, sae_sizeof(struct timeval));
        
        sae_memcpy(&event->event_timer_end, tv, sae_sizeof(struct timeval));
        event->event_timer_end.tv_sec += time(sae_null);

        if (!(event->event_timer_heap_elt = sae_heap_push(event->event_base->event_heap_timer, event)))
        {
            return sae_false;
        }
    }
    
    event->event_state = SAE_EVENT_STATE_INSERT;
    
    return sae_true;
}

sae_bool_t sae_event_del(sae_event_t *event)
{
    if (!(event->event_state & SAE_EVENT_STATE_INSERT))
    {
        return sae_false;
    }
    
    if (event->event_flag & SAE_EVENT_SIGNAL)
    {
        if (!sae_res_signal_set(event->event_fd,
                                &event->event_signal_old, sae_null))
        {
            sae_log(LERROR, "%s->%s sigid %d failed", "sae_event_del", "sae_res_set_signal", event->event_fd);
            return sae_false;
        }
        
        if (!sae_list_del(event->event_base->event_list_signal, event->event_signal_list_node))
        {
            return sae_false;
        }
    }
    else if (event->event_flag & SAE_EVENT_TIMER)
    {
        if (!sae_heap_del(event->event_base->event_heap_timer, event->event_timer_heap_elt))
        {
            return sae_false;
        }
    }
    else if (event->event_flag & SAE_EVENT_READ ||
             event->event_flag & SAE_EVENT_WRITE)
    {
        if (!event->event_base->event_base_module_select->del(event, event->event_base->event_base_module_select_instance))
        {
            sae_log(LERROR, "%s->%s failed clientsock=%d errno=%d", "sae_event_del", "del", event->event_fd, errno);
            return sae_false;
        }
        
        if (!sae_table_del(event->event_base->event_table_socket, event->event_fd_str))
        {
            return sae_false;
        }
    }
    
    event->event_state = SAE_EVENT_STATE_DELETE;
    
    return sae_true;
}

sae_bool_t sae_event_active(sae_event_t *event)
{
    if (event->event_state & SAE_EVENT_STATE_ACTIVE)
    {
        return sae_false;
    }
    
    event->event_state |= SAE_EVENT_STATE_ACTIVE;

    return sae_list_push(event->event_base->event_list_active, event) ? sae_true : sae_false;
}

sae_void_t sae_event_free(sae_event_t *event)
{
    sae_event_del(event);
    
    sae_alloc_free(event->event_fd_str);
    
    sae_alloc_free(event);
}

sae_event_base_t *sae_event_base_create(sae_event_top_t *module, sae_event_base_handle_call_back call)
{
    assert(module && call);
    
    sae_event_t *event = sae_null;
    sae_event_base_t *base = sae_null;
    
    if (!(base = sae_alloc(sae_sizeof(sae_event_base_t))))
    {
        return base;
    }
    
    base->event_base_fd_max = (sae_uint_t)sae_res_fd_max_get();
    
    if (!(base->event_table_socket = sae_table_create(base->event_base_fd_max)))
    {
        sae_alloc_free(base);
        return sae_null;
    }
    
    if (!(base->event_heap_timer = sae_heap_create(base->event_base_fd_max, sae_event_timer_call_cmp)))
    {
        sae_table_destroy(base->event_table_socket);
        sae_alloc_free(base);
        return sae_null;
    }
    
    if (!(base->event_list_signal = sae_list_create()))
    {
        sae_heap_destroy(base->event_heap_timer);
        sae_table_destroy(base->event_table_socket);
        sae_alloc_free(base);
        return sae_null;
    }
    
    if (!(base->event_list_active = sae_list_create()))
    {
        sae_list_destroy(base->event_list_signal);
        sae_heap_destroy(base->event_heap_timer);
        sae_table_destroy(base->event_table_socket);
        sae_alloc_free(base);
        return sae_null;
    }
    
    sae_memzero(base->event_signal_array_active, sae_sizeof(base->event_signal_array_active));
    base->event_signal_state = sae_false;
    
    /*def wait time*/
    base->event_base_timer_wait.tv_sec = 1;
    base->event_base_timer_wait.tv_usec = 0;
    
    /*save select module*/
    base->event_base_module_select = module;
    
    /*init module instance*/
    if (!(base->event_base_module_select_instance = base->event_base_module_select->create(sae_null)))
    {
        sae_list_destroy(base->event_list_active);
        sae_list_destroy(base->event_list_signal);
        sae_heap_destroy(base->event_heap_timer);
        sae_table_destroy(base->event_table_socket);
        sae_alloc_free(base);
        return sae_null;
    }
    
    /*create socket pair*/
    if (!sae_socket_pair(base->event_signal_sock_pair))
    {
        base->event_base_module_select->destroy(base, base->event_base_module_select_instance);
        sae_list_destroy(base->event_list_active);
        sae_list_destroy(base->event_list_signal);
        sae_heap_destroy(base->event_heap_timer);
        sae_table_destroy(base->event_table_socket);
        sae_alloc_free(base);
        return sae_null;
    }
    
    /*event handle call*/
    base->event_base_handle_call = call;
    
    base->event_base_work = sae_true;
    
    /*add socket pair read event*/
    if (!(event = sae_event_socket_set(base, base->event_signal_sock_pair[1],
                                       SAE_EVENT_READ | SAE_EVENT_PERSIST, sae_event_signal_call_reset, sae_null)) ||
        !sae_event_socket_add(event))
    {
        return sae_null;
    }
    
    base->event_signal_sock_pair_read = event;
    
    if (!(event = sae_event_signal_set(base, SIGTERM, 0, sae_event_base_call_stop, base)) ||
        !sae_event_signal_add(event))
    {
        return sae_null;
    }
    
    base->event_signal_call_stop = event;
    
    return base;
}

sae_void_t sae_event_base_destroy(sae_event_base_t *base)
{
    /*note 
        all event alloc and free must be user operate*/
    
    /*destroy sock_pair*/
    sae_event_free(base->event_signal_sock_pair_read);
    sae_socket_close(base->event_signal_sock_pair[0]);
    sae_socket_close(base->event_signal_sock_pair[1]);
    
    base->event_base_module_select->destroy(base, base->event_base_module_select_instance);
    
    sae_list_destroy(base->event_list_active);
    
    sae_event_free(base->event_signal_call_stop);
    sae_list_destroy(base->event_list_signal);
    
    sae_heap_destroy(base->event_heap_timer);
    sae_table_destroy(base->event_table_socket);
    sae_alloc_free(base);
}

static sae_bool_t sae_event_base_handle(sae_event_base_t *base)
{
    sae_event_t *event = sae_null;
    sae_list_node_t *node = sae_null;
    
    sae_list_for(base->event_list_active, node)
    {
        event = node->data;
        
        /*event signal reset, internal process*/
        if (event->event_flag & SAE_EVENT_READ &&
            event == base->event_signal_sock_pair_read)
        {
            event->event_call(event, sae_null);
            continue;
        }
        
        /*stop event base dispatch, internal process*/
        if (event->event_flag & SAE_EVENT_SIGNAL &&
            event == base->event_signal_call_stop)
        {
            event->event_call(event, base);
            continue;
        }
        
        if (!base->event_base_handle_call(event))
        {
            sae_log(LERROR, "%s->%s failed", "sae_event_base_handle", "event_base_handle_call");
        }
    }
    
    return sae_true;
}

sae_bool_t sae_event_base_dispatch(sae_event_base_t *base)
{
    while (base->event_base_work)
    {
        /*clear active list*/
        sae_list_clear(base->event_list_active);
        
        /*get read write event*/
        struct timeval min_timer = sae_event_timer_min_get(base, &base->event_base_timer_wait);
        
        if (!base->event_base_module_select->dispatch(base, &min_timer, base->event_base_module_select_instance))
        {
            sae_log(LERROR, "%s->%s failed errno:%d", "sae_event_base_dispatch", "dispatch", errno);
            return sae_false;
        }
        
        /*get timer event*/
        if (!sae_event_timer_active_get(base))
        {
            sae_log(LERROR, "%s->%s failed errno:%d", "sae_event_base_dispatch", "timer or signal handle", errno);
            return sae_false;
        }
        
        /*get signal event*/
        if (!sae_event_signal_active_get(base))
        {
            sae_log(LERROR, "%s->%s failed errno:%d", "sae_event_base_dispatch", "sae_event_signal_active_get", errno);
            return sae_false;
        }
        
        /*handle active event*/
        if (!sae_event_base_handle(base))
        {
            sae_log(LERROR, "%s->%s failed errno:%d", "sae_event_base_dispatch", "sae_event_base_handle", errno);
            return sae_false;
        }
        
        /*test*/
        break;
    }
    
    return sae_true;
}

sae_event_top_array_t *sae_event_top_create()
{
    const sae_ushort_t event_top_num = 4;
    sae_event_top_array_t *event_top_array = sae_array_create(event_top_num, sae_sizeof(sae_event_top_t));
    if (!event_top_array)
    {
        return event_top_array;
    }
    
#ifdef HAVE_KQUEUE
    extern const sae_event_top_t kqueue_top;
    sae_memcpy(sae_array_push_index(event_top_array, 0), &kqueue_top, sae_sizeof(sae_event_top_t));
#endif
    
#ifdef HAVE_EPOLL
    extern const sae_event_top_t epoll_top;
    sae_memcpy(sae_array_push_index(event_top_array, 1), &epoll_top, sae_sizeof(sae_event_top_t));
#endif
    
#ifdef HAVE_POLL
#endif
    
#ifdef HAVE_SELECT
#endif
    
    return event_top_array;
}

void sae_event_top_destroy(sae_event_top_array_t *array)
{
    sae_array_destroy(array);
}

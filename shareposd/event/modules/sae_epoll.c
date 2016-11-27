
/*
 * Copyright (C) hanchong liu
 */


#include "../sae_event.h"

#ifdef HAVE_EPOLL

typedef struct sae_epoll_s sae_epoll_t;

struct sae_epoll_s
{
    sae_int_t eid;/*epoll id*/
    struct epoll_event *event_list;/*event list*/
    sae_int_t event_list_len;/*event list len*/
};

static sae_void_t sae_epoll_event_set(struct epoll_event *eev, sae_event_fd_t fd, sae_int_t filter, sae_void_t *data)
{
    eev->data.fd = fd;
    eev->events = filter;
    eev->data.ptr = data;
}

sae_void_t *sae_epoll_create(sae_void_t *arg)
{
    sae_epoll_t *ep = sae_null;
    
    if (!(ep = sae_alloc(sae_sizeof(sae_epoll_t))))
    {
        return ep;
    }
    
    /* Initalize the kernel queue */
    if ((ep->eid = epoll_create(10)) == -1)
    {
        sae_alloc_free(ep);
        return sae_null;
    }
    
    /*init alloc len*/
    ep->event_list_len = 64;
    
    if (!(ep->event_list = sae_alloc_n(sae_sizeof(struct epoll_event), ep->event_list_len)))
    {
        sae_socket_close(ep->eid);
        sae_alloc_free(ep);
        return sae_null;
    }

    return ep;
}

sae_bool_t sae_epoll_add(sae_event_t *event, sae_void_t *arg)
{
    struct epoll_event eev;
    sae_int_t flags = 0;
    sae_int_t filter = 0;
    sae_epoll_t *ep = arg;
    
    if (event->event_flag & SAE_EVENT_READ)
    {
        filter = EPOLLIN;
    }
    else if (event->event_flag & SAE_EVENT_WRITE)
    {
        filter = EPOLLOUT;
    }
    else
    {
        return sae_false;
    }
    
    flags = EPOLL_CTL_ADD;
    flags |= (event->event_flag & SAE_EVENT_PERSIST) ? 0 : EPOLLONESHOT;
    
    sae_epoll_event_set(&eev, event->event_fd, filter, event);
    
    /*insert to kernel*/
    if (epoll_ctl(ep->eid, flags, event->event_fd, &eev) == -1)
    {
        return sae_false;
    }

    event->event_state = SAE_EVENT_STATE_INSERT;
    
    return sae_true;
}

sae_bool_t sae_epoll_del(sae_event_t *event, sae_void_t *arg)
{
    if (!(event->event_state & SAE_EVENT_STATE_INSERT))
    {
        return sae_false;
    }
    
    struct epoll_event eev;
    sae_int_t flags = 0;
    sae_int_t filter = 0;
    sae_epoll_t *ep = arg;
    
    if (event->event_flag & SAE_EVENT_READ)
    {
        filter = EPOLLIN;
    }
    else if (event->event_flag & SAE_EVENT_WRITE)
    {
        filter = EPOLLOUT;
    }
    else
    {
        return sae_false;
    }
    
    flags = EPOLL_CTL_DEL;
    
    sae_epoll_event_set(&eev, event->event_fd, filter, sae_null);

    /*del to kernel*/
    if (epoll_ctl(ep->eid, flags, event->event_fd, &eev) == -1)
    {
        return sae_false;
    }
    
    event->event_state |= SAE_EVENT_STATE_DELETE;
    
    return sae_true;
}

sae_bool_t sae_epoll_dispatch(sae_event_base_t *base, struct timeval *tv, sae_void_t *arg)
{
    sae_int_t i = 0;
    sae_int_t ret = -1;
    sae_epoll_t *ep = arg;
    struct epoll_event *event_list = ep->event_list;
    sae_event_t *event = sae_null;
    sae_int_t wait_time = (sae_int_t)(tv->tv_sec * 1000 + tv->tv_usec / 1000);
    
    ret = epoll_wait(ep->eid, event_list, ep->event_list_len, wait_time);
    if (ret == -1)
    {
        if (errno != EINTR)
        {
            return sae_false;
        }
        
        return sae_true;
    }
    
    for (i = 0; i < ret; i++)
    {
        if (event_list[i].events & EV_ERROR)
        {
            return sae_false;
        }
        
        if (event_list[i].events & EPOLLHUP)
        {
            continue;
        }
        
        event = (sae_event_t *)event_list[i].data.ptr;
        
        /*not read write event*/
        if (event_list[i].events & EPOLLIN &&
            event_list[i].events & EPOLLOUT)
        {
            continue;
        }
        
        /*not PERSIST and not delete*/
        if (!(event->event_flag & SAE_EVENT_PERSIST) &&
            !(event->event_state & SAE_EVENT_STATE_DELETE))
        {
            sae_event_del(event);
        }
        
        /*not active*/
        if (!(event->event_state & SAE_EVENT_STATE_ACTIVE))
        {
            event->event_state |= SAE_EVENT_STATE_ACTIVE;
            
            sae_event_active(event);
        }
    }
    
    return sae_true;
}

sae_bool_t sae_epoll_destroy(sae_event_base_t *base, sae_void_t *arg)
{
    sae_epoll_t *ep = arg;
    
    sae_alloc_free(ep->event_list);
    
    sae_socket_close(ep->eid);
    
    sae_alloc_free(ep);
    
    return sae_true;
}

const sae_event_top_t epoll_top =
{
    "epoll",
    sae_epoll_create,
    sae_epoll_add,
    sae_epoll_del,
    sae_epoll_dispatch,
    sae_epoll_destroy
};

#endif

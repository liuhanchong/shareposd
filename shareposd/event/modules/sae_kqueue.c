
/*
 * Copyright (C) hanchong liu
 */


#include "../sae_event.h"

#ifdef HAVE_KQUEUE

typedef struct sae_kqueue_s sae_kqueue_t;

struct sae_kqueue_s
{
    sae_int_t kid;/*kqueue id*/
    struct kevent *event_list;/*event list*/
    sae_int_t event_list_len;/*event list len*/
    struct kevent *event_list_change;/*insert event list*/
    sae_int_t event_list_change_len;/*insert event list len*/
    sae_int_t event_list_alloc;/*alloc event len*/
};

sae_bool_t sae_kqueue_insert(sae_kqueue_t *kq, struct kevent *kev)
{
    struct kevent *event_list_new = sae_null;
    struct kevent *event_list_change_new = sae_null;
    sae_int_t event_list_alloc_new = kq->event_list_alloc;
    
    if (kq->event_list_change_len == kq->event_list_alloc)
    {
        /*mem auto increate double*/
        event_list_alloc_new *= 2;
        
        if (!(event_list_new = sae_alloc_n(sae_sizeof(struct kevent), kq->event_list_alloc)))
        {
            return sae_false;
        }
        
        if (!(event_list_change_new = sae_alloc_n(sae_sizeof(struct kevent), kq->event_list_alloc)))
        {
            sae_alloc_free(event_list_new);
            return sae_false;
        }
        
        sae_memcpy(event_list_new, kq->event_list,
                   (sae_sizeof(struct kevent) * kq->event_list_len));
        
        sae_memcpy(event_list_change_new, kq->event_list_change,
                   (sae_sizeof(struct kevent) * kq->event_list_change_len));
        
        kq->event_list_change = event_list_change_new;
        kq->event_list = event_list_new;
        kq->event_list_alloc = event_list_alloc_new;
        kq->event_list_len = kq->event_list_alloc;
    }
    
    sae_memcpy(&kq->event_list_change[kq->event_list_change_len++], kev, sae_sizeof(struct kevent));
    
    return sae_true;
}

sae_void_t *sae_kqueue_create(sae_void_t *arg)
{
    sae_kqueue_t *kq = sae_null;
    
    if (!(kq = sae_alloc(sae_sizeof(sae_kqueue_t))))
    {
        return kq;
    }
    
    /* Initalize the kernel queue */
    if ((kq->kid = kqueue()) == -1)
    {
        sae_alloc_free(kq);
        return sae_null;
    }
    
    /*init alloc len*/
    kq->event_list_alloc = 64;
    
    if (!(kq->event_list = sae_alloc_n(sae_sizeof(struct kevent), kq->event_list_alloc)))
    {
        sae_socket_close(kq->kid);
        sae_alloc_free(kq);
        return sae_null;
    }
    
    if (!(kq->event_list_change = sae_alloc_n(sae_sizeof(struct kevent), kq->event_list_alloc)))
    {
        sae_socket_close(kq->kid);
        sae_alloc_free(kq->event_list);
        sae_alloc_free(kq);
        return sae_null;
    }

    kq->event_list_len = kq->event_list_alloc;
    kq->event_list_change_len = 0;

    return kq;
}

sae_bool_t sae_kqueue_add(sae_event_t *event, sae_void_t *arg)
{
    struct kevent kev;
    sae_int_t flags = 0;
    sae_int_t filter = 0;
    sae_kqueue_t *kq = arg;
    
    if (event->event_flag & SAE_EVENT_READ)
    {
        filter = EVFILT_READ;
    }
    else if (event->event_flag & SAE_EVENT_WRITE)
    {
        filter = EVFILT_WRITE;
    }
    else
    {
        return sae_false;
    }
    
    flags = EV_ADD;
    flags |= (event->event_flag & SAE_EVENT_PERSIST) ? 0 : EV_ONESHOT;
    
    EV_SET(&kev, event->event_fd, filter, flags, 0, 0, event);
    
    /*insert to kernel*/
    if (!sae_kqueue_insert(kq, &kev))
    {
        return sae_false;
    }

    event->event_state = SAE_EVENT_STATE_INSERT;
    
    return sae_true;
}

sae_bool_t sae_kqueue_del(sae_event_t *event, sae_void_t *arg)
{
    if (!(event->event_state & SAE_EVENT_STATE_INSERT))
    {
        return sae_false;
    }
    
    struct kevent kev;
    sae_int_t flags = 0;
    sae_int_t filter = 0;
    sae_kqueue_t *kq = arg;
    
    if (event->event_flag & SAE_EVENT_READ)
    {
        filter = EVFILT_READ;
    }
    else if (event->event_flag & SAE_EVENT_WRITE)
    {
        filter = EVFILT_WRITE;
    }
    else
    {
        return sae_false;
    }
    
    flags = EV_DELETE;
    
    EV_SET(&kev, event->event_fd, filter, flags, 0, 0, sae_null);

    /*del to kernel*/
    if (!sae_kqueue_insert(kq, &kev))
    {
        return sae_false;
    }
    
    event->event_state |= SAE_EVENT_STATE_DELETE;
    
    return sae_true;
}

sae_bool_t sae_kqueue_dispatch(sae_event_base_t *base, struct timeval *tv, sae_void_t *arg)
{
    sae_int_t i = 0;
    sae_int_t ret = -1;
    sae_kqueue_t *kq = arg;
    struct kevent *event_list = kq->event_list;
    sae_event_t *event = sae_null;
    
    struct timespec ts;
    sae_times_timeval_to_timespec(tv, &ts);
    
    ret = kevent(kq->kid, kq->event_list_change,
                 kq->event_list_change_len, event_list, kq->event_list_len, &ts);
    if (ret == -1)
    {
        if (errno != EINTR)
        {
            return sae_false;
        }
        
        return sae_true;
    }
    
    /*clear change list*/
    kq->event_list_change_len = 0;
    
    for (i = 0; i < ret; i++)
    {
        if (event_list[i].flags & EV_ERROR)
        {
            if (event_list[i].data == EBADF ||
                event_list[i].data == EINVAL ||
                event_list[i].data == ENOENT)
            {
                continue;
            }
            
            return sae_false;
        }
        
        event = (sae_event_t *)event_list[i].udata;
        
        /*not read write event*/
        if (event_list[i].filter != EVFILT_READ &&
            event_list[i].filter != EVFILT_WRITE)
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

sae_bool_t sae_kqueue_destroy(sae_event_base_t *base, sae_void_t *arg)
{
    sae_kqueue_t *kq = arg;
    
    sae_alloc_free(kq->event_list);
    
    sae_alloc_free(kq->event_list_change);
    
    sae_socket_close(kq->kid);
    
    sae_alloc_free(kq);
    
    return sae_true;
}

const sae_event_top_t kqueue_top =
{
    "kqueue",
    sae_kqueue_create,
    sae_kqueue_add,
    sae_kqueue_del,
    sae_kqueue_dispatch,
    sae_kqueue_destroy
};

#endif

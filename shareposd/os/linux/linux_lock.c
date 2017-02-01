
/*
 * Copyright (C) hanchong liu
 */


#include "linux_lock.h"

sae_bool_t sae_lock(sae_lock_t *lock)
{
    return (pthread_mutex_lock(lock) == 0)  ? sae_true : sae_false;
}

sae_bool_t sae_unlock(sae_lock_t *lock)
{
    return (pthread_mutex_unlock(lock) == 0)  ? sae_true : sae_false;
}



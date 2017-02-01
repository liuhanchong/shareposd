
/*
 * Copyright (C) hanchong liu
 */


#ifndef _LINUX_LOCK_H_INCLUDED_
#define _LINUX_LOCK_H_INCLUDED_

#include "../../core/sae_type.h"

sae_bool_t sae_lock(sae_lock_t *lock);

sae_bool_t sae_unlock(sae_lock_t *lock);

#endif /* _LINUX_LOCK_H_INCLUDED_ */


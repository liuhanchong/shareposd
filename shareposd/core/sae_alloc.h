
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_ALLOC_H_INCLUDED_
#define _SAE_ALLOC_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

sae_void_t *sae_alloc(sae_size_t size);

sae_void_t *sae_alloc_n(sae_size_t size, sae_uint_t nalloc);

sae_void_t  sae_alloc_free(sae_void_t *alloc);

#endif /* _SAE_ALLOC_H_INCLUDED_ */

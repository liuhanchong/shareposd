
/*
 * Copyright (C) hanchong liu
 */


#include "sae_config.h"
#include "sae_core.h"

sae_void_t *sae_alloc(sae_size_t size)
{
    return sae_alloc_n(size, 1);
}

sae_void_t *sae_alloc_n(sae_size_t size, sae_uint_t nalloc)
{
    return sae_malloc(size * nalloc);
}

sae_void_t sae_alloc_free(sae_void_t *alloc)
{
    sae_free(alloc);
}

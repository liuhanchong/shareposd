
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_ARRAY_H_INCLUDED_
#define _SAE_ARRAY_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

struct sae_array_s
{
    sae_void_t  *elts;
    sae_uint_t   nelts;/*cur elt number*/
    sae_size_t   size;/*a elt size*/
    sae_uint_t   nalloc;/*alloc elt number*/
};

sae_array_t *sae_array_create(sae_uint_t n, sae_size_t size);

sae_void_t   sae_array_destroy(sae_array_t *array);

#if 0
sae_void_t  *sae_array_push(sae_array_t *array);
#endif

sae_void_t  *sae_array_value_get(sae_array_t *array, sae_uint_t index);

sae_void_t  *sae_array_push_index(sae_array_t *array, sae_uint_t index);

#endif /* _SAE_ARRAY_H_INCLUDED_ */

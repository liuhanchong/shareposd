
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_HEAP_H_INCLUDED_
#define _SAE_HEAP_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

struct sae_heap_elt_s
{
    sae_void_t *data;
    sae_uint_t pos;/*from 1 begin*/
};

struct sae_heap_s
{
    sae_array_t *head;
    sae_uint_t   nelts;/*cur elt number*/
    sae_heap_elt_cmp cmp;/*cmp fun*/
};

sae_heap_t *sae_heap_create(sae_uint_t n, sae_heap_elt_cmp cmp);

sae_bool_t sae_heap_push(sae_heap_t *heap, sae_void_t *data);

sae_bool_t sae_heap_del(sae_heap_t *heap, sae_heap_elt_t *elt);

sae_void_t *sae_heap_value_min_get(sae_heap_t *heap);

sae_void_t sae_heap_destroy(sae_heap_t *heap);

sae_bool_t sae_heap_del_value(sae_heap_t *heap, sae_void_t *data);

sae_uint_t sae_heap_size_get(sae_heap_t *heap);

sae_void_t *sae_heap_value_get(sae_heap_t *heap, sae_uint_t index);

#endif /* _SAE_HEAP_H_INCLUDED_ */

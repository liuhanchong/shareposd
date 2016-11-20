
/*
 * Copyright (C) hanchong liu
 */


#include "sae_config.h"
#include "sae_core.h"

sae_array_t *sae_array_create(sae_uint_t n, sae_size_t size)
{
    /*alloc array*/
    sae_array_t *array = sae_alloc(sae_sizeof(sae_array_t));
    if (!array)
    {
        return array;
    }
    
    /*alloc elts*/
    array->elts = sae_alloc_n(size, n);
    if (!array->elts)
    {
        sae_alloc_free(array);
        return sae_null;
    }
    
    /*array init*/
    array->nalloc = n;
    array->nelts  = 0;
    array->size   = size;
    
    return array;
}

sae_void_t sae_array_destroy(sae_array_t *array)
{
    sae_alloc_free(array->elts);
    
    sae_alloc_free(array);
}

sae_void_t *sae_array_push(sae_array_t *array)
{
    return sae_array_push_index(array, array->nelts);
}

sae_void_t *sae_array_value_get(sae_array_t *array, sae_uint_t index)
{
    return ((sae_char_t *)array->elts + index * array->size);
}

sae_void_t *sae_array_push_index(sae_array_t *array, sae_uint_t index)
{
    if (array->nelts >= array->nalloc)
    {
        return sae_null;
    }
    
    ++(array->nelts);
    return ((sae_char_t *)array->elts + (index * array->size));
}

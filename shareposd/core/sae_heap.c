
/*
 * Copyright (C) hanchong liu
 */


#include "sae_heap.h"

static sae_heap_elt_t *sae_heap_elt_get(sae_heap_t *heap, sae_uint_t index)
{
    return sae_array_value_get(heap->head, index);
}

static sae_void_t sae_heap_elt_data_swap(sae_heap_elt_t *lelt, sae_heap_elt_t *relt)
{
    sae_void_t *data = lelt->data;
    lelt->data = relt->data;
    relt->data = data;
}

static sae_bool_t sae_heap_elt_move_up(sae_heap_t *heap, sae_heap_elt_t *elt)
{
    /*parent node*/
    sae_heap_elt_t *prt_elt = sae_null;
    
    if (elt->pos == 1)
    {
        return sae_true;
    }
    
    prt_elt = sae_heap_elt_get(heap, (elt->pos / 2 - 1));
    if (heap->cmp(elt, prt_elt) == 1)
    {
        /*swap data*/
        sae_heap_elt_data_swap(elt, prt_elt);
        
        return sae_heap_elt_move_up(heap, prt_elt);
    }
    
    return sae_true;
}

static sae_bool_t sae_heap_elt_move_down(sae_heap_t *heap, sae_heap_elt_t *elt)
{
    sae_heap_elt_t *cmp_elt = sae_null;
    
    if (elt->pos          > heap->nelts ||
       (elt->pos * 2)     > heap->nelts ||
       (elt->pos * 2 + 1) > heap->nelts)
    {
        return sae_true;
    }
    
    /*cmp left right elt*/
    cmp_elt = (heap->cmp(sae_heap_elt_get(heap, (elt->pos * 2 - 1)),
                                        sae_heap_elt_get(heap, (elt->pos * 2))) != -1) ?
                                            sae_heap_elt_get(heap, (elt->pos * 2 - 1)) : sae_heap_elt_get(heap, (elt->pos * 2));
    
    /*cmp cur elt and (left right min elt)*/
    if (heap->cmp(elt, cmp_elt) == -1)
    {
        /*swap data*/
        sae_heap_elt_data_swap(elt, cmp_elt);
        
        return sae_heap_elt_move_down(heap, cmp_elt);
    }
    
    return sae_true;
}

sae_heap_t *sae_heap_create(sae_uint_t n, sae_heap_elt_cmp cmp)
{
    sae_heap_t *heap = sae_alloc(sae_sizeof(sae_heap_t));
    if (!heap)
    {
        return heap;
    }
    
    heap->head = sae_array_create(n, sae_sizeof(sae_heap_elt_t));
    if (!heap->head)
    {
        sae_alloc_free(heap);
        return sae_null;
    }
    
    sae_memzero(heap->head->elts, heap->head->size * heap->head->nalloc);
 
    heap->cmp = cmp;
    heap->nelts = 0;
    
    return heap;
}

sae_bool_t sae_heap_push(sae_heap_t *heap, sae_void_t *data)
{
    sae_heap_elt_t *last_elt = sae_null;
    
    if (heap->nelts >= heap->head->nalloc)
    {
        return sae_false;
    }
    
    /*data set in last elt*/
    last_elt = sae_heap_elt_get(heap, heap->nelts++);
    last_elt->data = data;
    last_elt->pos = heap->nelts;
    
    return sae_heap_elt_move_up(heap, last_elt);
}

sae_bool_t sae_heap_del(sae_heap_t *heap, sae_heap_elt_t *elt)
{
    elt->data = sae_heap_elt_get(heap, (--heap->nelts))->data;
    
    return sae_heap_elt_move_down(heap, elt);
}

sae_void_t *sae_heap_value_min_get(sae_heap_t *heap)
{
    return (heap->nelts < 1) ? sae_null : sae_heap_elt_get(heap, 0)->data;
}

sae_void_t sae_heap_destroy(sae_heap_t *heap)
{
    sae_array_destroy(heap->head);
    
    sae_alloc_free(heap);
}

sae_bool_t sae_heap_del_value(sae_heap_t *heap, sae_void_t *data)
{
    sae_uint_t i = 0;
    sae_heap_elt_t *elt = sae_null;

    for (i = 0; i < heap->nelts; i++)
    {
        elt = sae_array_value_get(heap->head, i);
        if (elt->data == data)
        {
            return sae_heap_del(heap, elt);
        }
    }
    
    return sae_false;
}

sae_uint_t sae_heap_size_get(sae_heap_t *heap)
{
    return heap->nelts;
}

sae_void_t *sae_heap_value_get(sae_heap_t *heap, sae_uint_t index)
{
    return sae_array_value_get(heap->head, index);
}

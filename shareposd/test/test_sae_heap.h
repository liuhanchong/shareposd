//
//  test_sae_heap.h
//  shareposd
//
//  Created by liuhanchong on 2016/10/4.
//  Copyright © 2016年 liuhanchong. All rights reserved.
//

#ifndef test_sae_heap_h
#define test_sae_heap_h

sae_short_t elt_cmp(sae_heap_elt_t *src, sae_heap_elt_t *dst)
{
    /*0-eq 1-<  -1 >*/
    int nsrc = *(int *)src->data;
    int ndst = *(int *)dst->data;
    
    if (nsrc < ndst)
    {
        return 1;
    }
    else if (nsrc > ndst)
    {
        return -1;
    }
    
    return 0;
}

sae_int_t main_heap(sae_int_t argc, sae_char_t **argv)
{
    sae_heap_t *heap = sae_heap_create(10, elt_cmp);
    if (!heap)
    {
        printf("create heap failed\n");
        return 1;
    }
    
    int array[10] = {3, 4, 1, 8, 5, 2, 7, 10, 6, 9};
    
    for (int i = 0; i < 10; i++)
    {
        sae_heap_push(heap, &array[i]);
    }
    
    printf("min %d \n", *(int *)sae_heap_value_min_get(heap));
    
    for (int i = 0; i < heap->nelts; i++)
    {
        printf("%d ", *(int *)((sae_heap_elt_t *)sae_array_value_get(heap->head, i))->data);
    }
    printf("\n");
    
    sae_heap_rm(heap, sae_array_value_get(heap->head, 2));
    sae_heap_rm(heap, sae_array_value_get(heap->head, 4));
    sae_heap_rm(heap, sae_array_value_get(heap->head, 7));
    sae_heap_rm(heap, sae_array_value_get(heap->head, 4));
    sae_heap_rm(heap, sae_array_value_get(heap->head, 4));
    
    for (int i = 0; i < heap->nelts; i++)
    {
        printf("%d ", *(int *)((sae_heap_elt_t *)sae_array_value_get(heap->head, i))->data);
    }
    printf("\n");
    
    
    sae_heap_destroy(heap);
    
    return 0;
}

#endif /* test_sae_heap_h */

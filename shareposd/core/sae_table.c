
/*
 * Copyright (C) hanchong liu
 */


#include "sae_table.h"

static sae_bool_t sae_table_key_equal(sae_cchar_t *dkey, sae_cchar_t *skey)
{
    return (sae_strcmp(dkey, skey) == 0) ? sae_true : sae_false;
}

static sae_list_t *sae_table_list_get(sae_table_t *table, sae_uint_t index)
{
    return *(sae_list_t **)sae_array_value_get(table->table, index);
}

sae_table_t *sae_table_create(sae_uint_t n)
{
    /*alloc table*/
    sae_table_t *table = sae_alloc(sae_sizeof(sae_table_t));
    if (!table)
    {
        return table;
    }
    
    /*alloc array*/
    table->table = sae_array_create(n, sae_sizeof(sae_list_t *));
    if (!table->table)
    {
        sae_alloc_free(table);
        return sae_null;
    }
    
    /*init mem*/
    sae_memzero(table->table->elts, table->table->nalloc * table->table->size);
    
    return table;
}

sae_void_t sae_table_destroy(sae_table_t *table)
{
    sae_uint_t i = 0;
    sae_list_t *list = sae_null;
    sae_list_node_t *node = sae_null;
    
    /*free list node*/
    for (i = 0; i < table->table->nalloc; i++)
    {
        list = sae_table_list_get(table, i);
        if (list)
        {
            /*free elt*/
            node = list->head;
            while (node)
            {
                sae_alloc_free(node->data);
                node = node->next;
            }
            
            sae_list_destroy(list);
        }
    }
    
    /*free arary*/
    sae_array_destroy(table->table);
    
    sae_alloc_free(table);
}

sae_bool_t sae_table_push(sae_table_t *table, sae_char_t *key, sae_void_t *value)
{
    sae_uint_t index = 0;
    sae_list_t *list = sae_null;
    sae_table_elt_t *elt = sae_null;
    
    elt = sae_alloc(sae_sizeof(sae_table_elt_t));
    if (!elt)
    {
        return sae_false;
    }
    
    elt->key = key;
    elt->value = value;
    
    index = (sae_uint_t)(sae_str_hash(key) % table->table->nalloc);
    list = sae_table_list_get(table, index);
    if (!list)
    {
        /*alloc list*/
        list = sae_list_create();
        if (!list)
        {
            sae_alloc_free(elt);
            return sae_false;
        }
        
        /*add list*/
        sae_memcpy(sae_array_push_index(table->table, index),
                   &list, sae_sizeof(sae_list_t *));
    }

    /*add elt*/
    sae_list_push(list, elt);
    
    return sae_true;
}

sae_bool_t sae_table_del(sae_table_t *table, sae_char_t *key)
{
    sae_list_t *list = sae_null;
    sae_list_node_t *node = sae_null;
    sae_table_elt_t *elt = sae_null;
    
    list = sae_table_list_get(table,
                              (sae_uint_t)(sae_str_hash(key) % table->table->nalloc));
    node = list->head;
    while (node)
    {
        elt = node->data;
        if (sae_table_key_equal(elt->key, key))
        {
            sae_alloc_free(elt);
            sae_list_del(list, node);
            return sae_true;
        }
        node = node->next;
    }

    return sae_false;
}

sae_void_t *sae_table_value(sae_table_t *table, sae_char_t *key)
{
    sae_list_t *list = sae_null;
    sae_list_node_t *node = sae_null;
    sae_table_elt_t *elt = sae_null;
    
    if (!(list = sae_table_list_get(table,
                                    (sae_uint_t)(sae_str_hash(key) % table->table->nalloc))))
    {
        return sae_null;
    }
    
    node = list->head;
    while (node)
    {
        elt = node->data;
        if (sae_table_key_equal(elt->key, key))
        {
            return elt->value;
        }
        node = node->next;
    }
    
    return sae_null;
}

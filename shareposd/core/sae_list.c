
/*
 * Copyright (C) hanchong liu
 */


#include "sae_log.h"

sae_list_t *sae_list_create()
{
    sae_list_t *list = sae_alloc(sae_sizeof(sae_list_t));
    if (!list)
    {
        return list;
    }
    
    list->head = sae_null;
    list->last = sae_null;
    
    return list;
}

sae_void_t sae_list_destroy(sae_list_t *list)
{
    sae_list_node_t *node = list->head;
    sae_list_node_t *next_node = sae_null;
    
    /*free list node*/
    while (node)
    {
        next_node = node->next;
        sae_alloc_free(node);
        node = next_node;
    }
    
    sae_alloc_free(list);
}

sae_list_node_t *sae_list_push(sae_list_t *list, sae_void_t *data)
{
    /*alloc node*/
    sae_list_node_t *node = sae_alloc(sae_sizeof(sae_list_node_t));
    if (!node)
    {
        return sae_null;
    }
    
    /*init node*/
    node->next = sae_null;
    node->data = data;
    
    /*insert new node*/
    if (sae_list_empty(list))
    {
        list->head = node;
    }
    else
    {
        list->last->next = node;
    }
    
    /*move last*/
    list->last = node;
    
    return node;
}

sae_bool_t sae_list_del(sae_list_t *list, sae_list_node_t *node)
{
    sae_list_node_t *pre_node = sae_null;
    sae_list_node_t *head_node = list->head;
    
    while (head_node)
    {
        if (head_node == node)
        {
            break;
        }
        
        pre_node = head_node;
        head_node = head_node->next;
    }
    
    /*no find*/
    if (head_node != node)
    {
        return sae_false;
    }
    
    if (head_node == list->head)
    {
        list->head = head_node->next;
    }
    
    if (head_node == list->last)
    {
        list->last = pre_node;
    }
    
    if (pre_node)
    {
        pre_node->next = head_node->next;
    }
    
    sae_alloc_free(node);
    
    return sae_true;
}

sae_bool_t sae_list_empty(sae_list_t *list)
{
    return (list->head) ? sae_false : sae_true;
}

sae_bool_t sae_list_del_value(sae_list_t *list, sae_void_t *data)
{
    sae_list_node_t *node = sae_null;
    
    sae_list_for (list, node)
    {
        if (node->data == data)
        {
            return sae_list_del(list, node);
        }
    }
    
    return sae_false;
}
sae_bool_t sae_list_clear(sae_list_t *list)
{
    while (!sae_list_empty(list))
    {
        sae_list_del(list, list->head);
    }
    
    return sae_true;
}

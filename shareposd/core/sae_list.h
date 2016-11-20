
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_LIST_H_INCLUDED_
#define _SAE_LIST_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

struct sae_list_node_s
{
    sae_void_t *data;
    sae_list_node_t *next;
};

struct sae_list_s
{
    sae_list_node_t *head;
    sae_list_node_t *last;
};

sae_list_t *sae_list_create();

sae_void_t sae_list_destroy(sae_list_t *list);

sae_bool_t sae_list_push(sae_list_t *list, sae_void_t *data);

sae_bool_t sae_list_del(sae_list_t *list, sae_list_node_t *node);

sae_bool_t sae_list_empty(sae_list_t *list);

sae_bool_t sae_list_del_value(sae_list_t *list, sae_void_t *data);

sae_bool_t sae_list_clear(sae_list_t *list);

#define sae_list_for(list, node) \
    for ((node) = ((list)->head); (node); (node) = ((node)->next))

#endif /* _SAE_LIST_H_INCLUDED_ */

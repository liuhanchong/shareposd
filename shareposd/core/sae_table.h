
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_TABLE_H_INCLUDED_
#define _SAE_TABLE_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

struct sae_table_elt_s
{
    sae_char_t *key;
    sae_void_t *value;
};

struct sae_table_s
{
    sae_array_t *table;
};

sae_table_t *sae_table_create(sae_uint_t n);

sae_void_t sae_table_destroy(sae_table_t *table);

sae_bool_t sae_table_push(sae_table_t *table, sae_char_t *key, sae_void_t *value);

sae_bool_t sae_table_del(sae_table_t *table, sae_char_t *key);

sae_void_t *sae_table_value(sae_table_t *table, sae_char_t *key);

#endif /* _SAE_TABLE_H_INCLUDED_ */

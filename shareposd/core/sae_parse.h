
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_PARSE_H_INCLUDED_
#define _SAE_PARSE_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

struct sae_parse_s
{
    sae_file_t *file;
};

sae_parse_t *sae_parse_create(sae_cchar_t *path);

sae_bool_t sae_parse_string(sae_parse_t *parse, sae_parse_char_t *section,
                            sae_parse_char_t *key, sae_char_t *value, sae_size_t size);

sae_int_t sae_parse_int(sae_parse_t *parse, sae_parse_char_t *section,
                                            sae_parse_char_t *key, sae_int_t def);

sae_float_t sae_parse_float(sae_parse_t *parse, sae_parse_char_t *section,
                                            sae_parse_char_t *key, sae_float_t def);

sae_void_t sae_parse_destroy(sae_parse_t *parse);

sae_bool_t sae_parse_set(sae_parse_t *parse, sae_parse_char_t *section, sae_parse_char_t *key,
                                                    sae_char_t *value, sae_size_t size);

sae_bool_t sae_parse_section_del(sae_parse_t *parse, sae_parse_char_t *section);

sae_bool_t sae_parse_key_del(sae_parse_t *parse, sae_parse_char_t *section, sae_parse_char_t *key);

#endif /* _SAE_PARSE_H_INCLUDED_ */

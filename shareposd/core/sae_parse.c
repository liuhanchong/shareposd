
/*
 * Copyright (C) hanchong liu
 */


#include "sae_config.h"
#include "sae_core.h"

static sae_bool_t sae_parse_section_find(sae_char_t *line, sae_parse_char_t *section)
{
    /*find section*/
    sae_size_t size = sae_strlen(section);
    sae_char_t *left = sae_strrchr(line, '[');
    sae_char_t *right = sae_strrchr(line, ']');
    if (!left || !right || size < 1)
    {
        return sae_false;
    }
    
    /*rm [*/
    left += 1;
    
    /*rm ]*/
    *right = sae_str_end;
    
    /*cmp section*/
    return (sae_strncmp_parse(left, section, size) == 0) ? sae_true : sae_false;
}

static sae_bool_t sae_parse_key_find(sae_char_t *line, sae_parse_char_t *key,
                                            sae_char_t *value, sae_size_t size)
{
    sae_char_t *equ = sae_null;
    sae_size_t key_size = sae_strlen(key);
    if (key_size < 1)
    {
        return sae_false;
    }
    
    /*find key*/
    equ = sae_strrchr(line, '=');
    if (!equ)
    {
        return sae_false;
    }
    
    /*cp value note:def value->size > line->value size*/
    sae_strncpy(value, equ + 1, size);
    
    /*rm space*/
    sae_str_trim(value);

    /*rm space*/
    *equ = sae_str_end;
    sae_str_trim(line);

    /*cmp key*/
    return (sae_strncmp_parse(line, key, key_size) == 0) ? sae_true : sae_false;
}

sae_parse_t *sae_parse_create(sae_cchar_t *path)
{
    sae_parse_t *parse = sae_alloc(sae_sizeof(sae_parse_t));
    if (!parse)
    {
        return parse;
    }
    
    parse->file = sae_file_open(path, SAE_FILE_READ);
    if (!parse->file)
    {
        sae_alloc_free(parse);
        
        return sae_null;
    }
    
    return parse;
}

sae_bool_t sae_parse_string(sae_parse_t *parse, sae_parse_char_t *section,
                            sae_parse_char_t *key, sae_char_t *value, sae_size_t size)
{
    sae_size_t read_len = 0;
    sae_char_t line[SAE_PARSE_ROW_MAX_LEN];
    sae_bool_t find_section = sae_false;
    sae_char_t *find = sae_null;
    
    /*set read pos to head*/
    sae_file_seek_begin(parse->file->fd);
    
    /*read line*/
    while ((read_len = sae_file_read_line(parse->file->fd,
                                     line, SAE_PARSE_ROW_MAX_LEN)) > 0)
    {
        /*rm note # flag is note*/
        find = sae_strrchr(line, '#');
        if (find)
        {
            *find = sae_str_end;
        }
        
        /*rm space*/
        sae_str_trim(line);
        
        /*find section*/
        if (!find_section)
        {
            find_section = sae_parse_section_find(line, section);
            continue;
        }
        
        /*find key*/
        if (sae_parse_key_find(line, key, value, size))
        {
            return sae_true;
        }
    }
    
    return sae_false;
}

sae_int_t sae_parse_int(sae_parse_t *parse, sae_parse_char_t *section,
                                    sae_parse_char_t *key, sae_int_t def)
{
    sae_char_t value[30] = {0};
    return (sae_parse_string(parse, section, key,
                                value, sae_sizeof(value))) ? atoi(value) : def;
}

sae_float_t sae_parse_float(sae_parse_t *parse, sae_parse_char_t *section,
                                        sae_parse_char_t *key, sae_float_t def)
{
    sae_char_t value[40] = {0};
    return (sae_parse_string(parse, section, key,
                                value, sae_sizeof(value))) ? (sae_float_t)atof(value) : def;
}

sae_void_t sae_parse_destroy(sae_parse_t *parse)
{
    sae_file_close(parse->file);
    
    sae_alloc_free(parse);
}

sae_bool_t sae_parse_set(sae_parse_t *parse, sae_parse_char_t *section, sae_parse_char_t *key,
                         sae_char_t *value, sae_size_t size)
{
    return sae_false;
}

sae_bool_t sae_parse_section_del(sae_parse_t *parse, sae_parse_char_t *section)
{
    return sae_false;
}

sae_bool_t sae_parse_key_del(sae_parse_t *parse, sae_parse_char_t *section, sae_parse_char_t *key)
{
    return sae_false;
}

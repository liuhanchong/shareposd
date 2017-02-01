
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_LOG_H_INCLUDED_
#define _SAE_LOG_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

enum sae_log_type_e
{
    LDEBUG = 0,
    LERROR,
    LOTHER,
    LDUMP,
    /*add new type there, not in LTYPE back*/
    LTYPE,
    LEXIT /*special type, therfore not in LTYPE before add*/
};

struct sae_log_s
{
    sae_char_t *dir;
    sae_file_t *files[SAE_LOG_TYPE_COUNT]; /*every type fd*/
    sae_bool_t valid;
};

/*special process global arg*/
static sae_log_t *sae_log_gl = sae_null;

/*
sae_log_t *sae_log_create();

sae_void_t sae_log_destroy(sae_log_t *log);
*/

sae_void_t sae_log_write(sae_log_t *log, sae_log_type_t log_type, sae_cchar_t *format, ...);

#define sae_log(logtype, format, ...) \
            sae_log_write(sae_log_gl, logtype, format, ##__VA_ARGS__)

#endif /* _SAE_LOG_H_INCLUDED_ */

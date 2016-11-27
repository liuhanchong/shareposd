
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_UTILS_H_INCLUDED_
#define _SAE_UTILS_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

sae_bool_t sae_utils_process_deamon();

sae_uint32_t sae_utils_crc(sae_char_t *data, sae_size_t len);

#endif /* _SAE_UTILS_H_INCLUDED_ */

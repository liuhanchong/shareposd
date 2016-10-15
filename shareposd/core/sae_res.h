
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_UTIL_H_INCLUDED_
#define _SAE_UTIL_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

sae_bool_t sae_res_fd_max_set(sae_uint64_t n);

sae_uint64_t sae_res_fd_max_get();

sae_bool_t sae_res_core_size_set(sae_uint32_t s);

sae_uint32_t sae_res_core_size_get();

sae_ushort_t sae_res_cpu_num_get();

sae_bool_t sae_res_signal_set(sae_int_t sig, sae_signal_t *new, sae_signal_t *old);

#endif /* _SAE_UTIL_H_INCLUDED_ */

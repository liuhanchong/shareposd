
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_CYCLE_H_INCLUDED_
#define _SAE_CYCLE_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

struct sae_cycle_core_s
{
    sae_int_t event_flag;/*0x01*/
    sae_void_t *event_top_array;/*sys module*/
    sae_char_t *ip;
    sae_uint16_t port;
};

struct sae_cycle_conf_s
{
	sae_int_t a;
    sae_cycle_core_t *cycle_core;
};

#endif /* _SAE_CYCLE_H_INCLUDED_ */

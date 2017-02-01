
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_CONFIG_H_INCLUDED_
#define _SAE_CONFIG_H_INCLUDED_

#include "config.h"
#include "sae_type.h"

#if (HAVE_WIN32)
#include "../os/win/win_base.h"
#include "../os/win/win_thread.h"
#include "../os/win/win_lock.h"
#include "../os/win/win_file.h"
#else
#include "../os/linux/linux_base.h"
#include "../os/linux/linux_thread.h"
#include "../os/linux/linux_lock.h"
#include "../os/linux/linux_file.h"
#endif

#endif /* _SAE_CONFIG_H_INCLUDED_ */

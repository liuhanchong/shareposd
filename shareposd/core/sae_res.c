
/*
 * Copyright (C) hanchong liu
 */


#include "sae_config.h"
#include "sae_core.h"

sae_bool_t sae_res_fd_max_set(sae_uint64_t n)
{
#if (HAVE_WIN32)
#error sae_res_fd_max_set
#else
    struct rlimit rlt;
    if (getrlimit(RLIMIT_NOFILE, &rlt) == 0)
    {
        rlt.rlim_cur = (n >= rlt.rlim_max) ? rlt.rlim_max : n;
        if (setrlimit(RLIMIT_NOFILE, &rlt) == 0)
        {
            return sae_true;
        }
    }
    
    return sae_false;
#endif
}

sae_uint64_t sae_res_fd_max_get()
{
#if (HAVE_WIN32)
#error sae_res_fd_max_get
#else
    struct rlimit rlt;
    return (getrlimit(RLIMIT_NOFILE, &rlt) == 0) ? (sae_uint64_t)rlt.rlim_cur : 256;
#endif
}

sae_bool_t sae_res_core_size_set(sae_uint32_t s)
{
#if (HAVE_WIN32)
#error sae_res_core_size_set
#else
    struct rlimit rlt;
    if (getrlimit(RLIMIT_CORE, &rlt) == 0)
    {
        rlt.rlim_cur = (s >= rlt.rlim_max) ? rlt.rlim_max : s;
        if (setrlimit(RLIMIT_CORE, &rlt) == 0)
        {
            return sae_true;
        }
    }
    
    return sae_false;
#endif
}

sae_uint32_t sae_res_core_size_get()
{
#if (HAVE_WIN32)
#error sae_res_core_size_get
#else
    struct rlimit rlt;
    return (getrlimit(RLIMIT_CORE, &rlt) == 0) ? (sae_uint32_t)rlt.rlim_cur : 0;
#endif
}

sae_ushort_t sae_res_cpu_num_get()
{
#if (HAVE_WIN32)
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors;
#else
    return (sae_ushort_t)sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

sae_bool_t sae_res_signal_set(sae_int_t sig, sae_signal_t *new, sae_signal_t *old)
{
#if (HAVE_WIN32)
#error sae_res_signal_set
#else
    return (sigaction(sig, new, old) == -1) ? sae_false : sae_true;
#endif
}

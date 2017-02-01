
/*
 * Copyright (C) hanchong liu
 */


#include "sae_log.h"

/*create log file*/
static sae_file_t *sae_log_file(sae_log_t *log, sae_log_type_t type)
{
    /*get log name*/
    sae_char_t *name = (type == LDEBUG)   ? "DEBUG" :
                        (type == LERROR)  ? "ERROR" :
                        (type == LDUMP)   ? "DUMP"  :
                        (type == LOTHER)  ? "OTHER" :
                                            "UNKNOWN";
    
    /*gen log path*/
    sae_char_t path[SAE_LOG_PATH_LEN] = {0};
    sae_snprintf(path, SAE_LOG_PATH_LEN, "%s%cSAE_%s", log->dir, sae_path_seg, name);
    
    return sae_file_open(path, SAE_FILE_WRITE | SAE_FILE_AUTO_CRE);
}

sae_log_t *sae_log_create(sae_char_t *dir)
{
    sae_size_t len = 0;
    sae_log_t *log = sae_null;
    sae_log_type_t type = LDEBUG;
    
    /*alloc log*/
    log = sae_alloc(sae_sizeof(sae_log_t));
    if (!log)
    {
        return log;
    }
    sae_memzero(log, sae_sizeof(sae_log_t));
    
    /*cp logdir*/
    len = sae_strlen(dir);
    log->dir = sae_alloc(len + 1);
    if (!log->dir)
    {
        sae_alloc_free(log);
        return sae_null;
    }
    sae_cp_str(log->dir, dir, len);
    
    /*create log dir*/
    if (!sae_dir_exist(log->dir))
    {
        sae_dir_create(log->dir);
    }
    else
    {
        printf("warn: create log dir is exist, log name is %s. new log file will cover old log file\n", log->dir);
    }
    
    /*gen file*/
    for (type = LDEBUG; type < LTYPE; ++type)
    {
        log->files[type] = sae_log_file(log, type);
        if (!log->files[type])
        {
            printf("error: create %d log failed!\n", type);
        }
    }
    
    log->valid = sae_true;
    
    return log;
}

sae_void_t sae_log_destroy(sae_log_t *log)
{
    sae_log_type_t type = LDEBUG;
    
    log->valid = sae_false;
    
    /*close log file*/
    for (type = LDEBUG; type < LTYPE; ++type)
    {
        sae_file_close(log->files[type]);
    }
    
    /*free path*/
    sae_alloc_free(log->dir);
    
    sae_alloc_free(log);
}

sae_void_t sae_log_write(sae_log_t *log, sae_log_type_t log_type, sae_cchar_t *format, ...)
{
    va_list arg_list;
    sae_cint_t timer_len = 30;
    sae_char_t stimer[timer_len] = {0};
    time_t timer = 0;
    struct tm *strutm = sae_null;
    sae_char_t info[SAE_LOG_SIZE] = {0};
    sae_char_t log_com[SAE_LOG_SIZE];
    sae_size_t len = 0;
    sae_file_t *file = sae_null;
    sae_bool_t log_exit = sae_false;
    
    /*auto exit log*/
    if (log_type == LEXIT)
    {
        log_type = LDEBUG;
        log_exit = sae_true;
    }
    
    /*special process log*/
    if (!sae_log_gl)
    {
#if (HAVE_WIN32)
        sae_log_gl = sae_log_create(".\\SAE_LOG");
#else
        sae_log_gl = sae_log_create("./SAE_LOG");
#endif
    }
    log = sae_log_gl;
    
    /*is printf debug log*/
#ifndef SAE_LOG_DEBUG
    if (logtype == LDEBUG)
    {
        return;
    }
#endif
    
    /*gen complete log info*/
    va_start(arg_list, format);
    
    /*gen time*/
    timer = time(sae_null);
    strutm = localtime(&timer);
    strftime(stimer, timer_len, "%Y-%m-%d %H:%M:%S", strutm);
    
    /*gen log info*/
    sae_vsnprintf(info, SAE_LOG_SIZE, format, arg_list);
    
    /*gen complete log*/
    len = sae_snprintf(log_com, SAE_LOG_SIZE, "%s: %s\n", stimer, info);
    
    va_end(arg_list);
    
    /*write log*/
    file = log->files[log_type];
    sae_file_write(file->fd, log_com, len);
    
    /*sync file*/
    sae_file_sync(file->fd);
    
    if (log_exit)
    {
        sae_log(LDEBUG, "log auto exit");
        sae_log_destroy(sae_log_gl);
    }
}

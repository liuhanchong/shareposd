
/*
 * Copyright (C) hanchong liu
 */


#include "sae_config.h"
#include "sae_core.h"

sae_bool_t sae_utils_process_deamon()
{
    /*fork new process*/
    sae_pid_t pid = sae_res_process_fork();
    if (pid > 0)/*parent pro*/
    {
        sae_res_process_exit(0);
    }
    else if (pid < 0)/*fork failed*/
    {
        return sae_false;
    }
    
    /*set new session*/
    pid = sae_res_session_set();
    if (pid < 0)
    {
        return sae_false;
    }
    
    /*set root dir, note no change*/
    
    /*set mask, note no change*/
    
    /*close fd*/
    return (sae_file_fd_close(STDIN_FILENO)  &&
            sae_file_fd_close(STDOUT_FILENO) &&
            sae_file_fd_close(STDERR_FILENO)) ? sae_true : sae_false;
}

sae_uint32_t sae_utils_crc(sae_char_t *data, sae_size_t len)
{
    return 0;
}

sae_void_t sae_utils_process_exit(sae_int_t status)
{
    exit(status);
}

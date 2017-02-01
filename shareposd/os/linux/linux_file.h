
/*
 * Copyright (C) hanchong liu
 */


#ifndef _LINUX_FILE_H_INCLUDED_
#define _LINUX_FILE_H_INCLUDED_

#include "../../core/sae_type.h"
#include "../../core/sae_core.h"

struct sae_file_s
{
    sae_file_fd_t fd;
    sae_char_t  *path;
    sae_bool_t   valid;/*file valid*/
};

sae_file_t *sae_file_open(sae_cchar_t *path, sae_file_op_t op);

sae_bool_t sae_file_fd_close(sae_file_fd_t fd);

sae_void_t sae_file_close(sae_file_t *file);

sae_ssize_t sae_file_read(sae_file_fd_t fd, sae_char_t *text, sae_size_t len);

sae_ssize_t sae_file_read_line(sae_file_fd_t fd, sae_char_t *text, sae_size_t len);

sae_ssize_t sae_file_write(sae_file_fd_t fd, sae_cchar_t *text, sae_size_t len);

sae_bool_t sae_file_exist(sae_cchar_t *path);

sae_bool_t sae_file_rm(sae_cchar_t *path);

sae_long_t sae_file_len(sae_file_fd_t fd);

sae_void_t sae_file_sync(sae_file_fd_t fd);

sae_void_t sae_file_seek_begin(sae_file_fd_t fd);

sae_bool_t sae_dir_exist(sae_cchar_t *path);

sae_bool_t sae_dir_create(sae_cchar_t *path);

#endif /* _LINUX_FILE_H_INCLUDED_ */


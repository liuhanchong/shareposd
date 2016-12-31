
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_BUFFER_H_INCLUDED_
#define _SAE_BUFFER_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

struct sae_buffer_s
{
    sae_char_t *buffer;
    sae_size_t buffer_max;
    sae_size_t buffer_len;
    sae_size_t buffer_off;
    sae_size_t buffer_off_line;
};

sae_buffer_t *sae_buffer_create();

sae_void_t sae_buffer_destroy(sae_buffer_t *buffer);

sae_bool_t sae_buffer_read_socket(sae_buffer_t *buffer, sae_socket_fd_t fd);

sae_bool_t sae_buffer_read_file(sae_buffer_t *buffer, sae_file_fd_t fd);

sae_char_t *sae_buffer_read_line(sae_buffer_t *buffer);

sae_void_t sae_buffer_read_line_free(sae_char_t *line);

sae_char_t *sae_buffer_get(sae_buffer_t *buffer);

sae_bool_t sae_buffer_add(sae_buffer_t *buffer, sae_char_t *buf, sae_size_t len);

sae_bool_t sae_buffer_add_printf(sae_buffer_t *buffer, sae_char_t *format, ...);

sae_buffer_t *sae_buffer_copy(sae_buffer_t *buffer);

#endif /* _SAE_BUFFER_H_INCLUDED_ */

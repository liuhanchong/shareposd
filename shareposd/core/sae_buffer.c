
/*
 * Copyright (C) hanchong liu
 */


#include "sae_buffer.h"

static sae_bool_t sae_buffer_realloc(sae_buffer_t *buffer, sae_size_t add_len)
{
    sae_void_t *buffer_new = sae_null;
    
    if (buffer->buffer_len >= buffer->buffer_max)
    {
        return sae_false;
    }
    
    buffer->buffer_len += add_len;
    
    buffer->buffer_len = (buffer->buffer_len > buffer->buffer_max) ? buffer->buffer_max : buffer->buffer_len;
    
    if (!(buffer_new = sae_realloc(buffer->buffer, buffer->buffer_len)))
    {
        return sae_false;
    }
    
    buffer->buffer = buffer_new;
    
    return sae_true;
}

sae_buffer_t *sae_buffer_create()
{
    sae_buffer_t *buffer = sae_alloc(sae_sizeof(sae_buffer_t));
    if (!buffer)
    {
        return buffer;
    }
    
    buffer->buffer_len = SAE_BUFFER_INIT_LEN;
    buffer->buffer_max = SAE_BUFFER_MAX_LEN;
    buffer->buffer_off = 0;
    buffer->buffer_off_line = 0;
    
    if (!(buffer->buffer = sae_alloc(buffer->buffer_len)))
    {
        sae_alloc_free(buffer);
        return sae_null;
    }
    
    return buffer;
}

sae_void_t sae_buffer_destroy(sae_buffer_t *buffer)
{
    sae_alloc_free(buffer->buffer);
    sae_alloc_free(buffer);
}

sae_bool_t sae_buffer_read_socket(sae_buffer_t *buffer, sae_socket_fd_t fd)
{
    sae_ssize_t len = 0;
    sae_size_t left = 0;
    
    while (sae_true)
    {
        //no space
        if (((left = buffer->buffer_len - buffer->buffer_off) < 1) &&
            !sae_buffer_realloc(buffer, SAE_BUFFER_INIT_LEN))
        {
            return sae_false;
        }
        
        if ((len = sae_socket_recv(fd, buffer->buffer + buffer->buffer_off, left, 0)) < 1)
        {
            if (len < 0)
            {
                sae_log(LERROR, "sae_buffer_read_socket read len = %d, fd is %d", len, fd);
                return sae_false;
            }
            break;
        }
        
        buffer->buffer_off += len;
    }
    
    return sae_true;
}

sae_bool_t sae_buffer_read_file(sae_buffer_t *buffer, sae_file_fd_t fd)
{
    sae_ssize_t len = 0;
    sae_size_t left = buffer->buffer_off;
    
    while (sae_true)
    {
        //no space
        if (((left = buffer->buffer_len - buffer->buffer_off) < 1) &&
            !sae_buffer_realloc(buffer, SAE_BUFFER_INIT_LEN))
        {
            return sae_false;
        }
        
        if ((len = sae_file_read(fd, buffer->buffer + buffer->buffer_off, left)) < 1)
        {
            if (len < 0)
            {
                sae_log(LERROR, "sae_buffer_read_file read len = %d, fd is %d", len, fd);
                return sae_false;
            }
            break;
        }
        
        buffer->buffer_off += len;
    }
    
    return sae_true;
}

sae_char_t *sae_buffer_read_line(sae_buffer_t *buffer)
{
    /*read line to last reset*/
    if (buffer->buffer_off_line >= buffer->buffer_off)
    {
        buffer->buffer_off_line = 0;
        return sae_null;
    }
    
    sae_size_t buffer_off_line_old = buffer->buffer_off_line;
    sae_size_t len = buffer->buffer_off - buffer->buffer_off_line;
    sae_char_t *buf = sae_alloc(len + 1);
    if (!buf)
    {
        return buf;
    }
    
    while (buffer->buffer_off_line < buffer->buffer_off)
    {
        if (sae_strchr(SAE_BUFFER_LINE_FLAG, buffer->buffer[buffer->buffer_off_line++]))
        {
            break;
        }
    }

    sae_memcpy(buf, buffer->buffer + buffer_off_line_old, buffer->buffer_off_line - buffer_off_line_old);
    buf[buffer->buffer_off_line - buffer_off_line_old - 1] = sae_str_end;
    
    return buf;
}

sae_void_t sae_buffer_read_line_free(sae_char_t *line)
{
    sae_alloc_free(line);
}

sae_char_t *sae_buffer_get(sae_buffer_t *buffer)
{
    return buffer->buffer;
}

sae_bool_t sae_buffer_add(sae_buffer_t *buffer, sae_char_t *buf, sae_size_t len)
{
    if ((buffer->buffer_max - buffer->buffer_off) < len)
    {
        return sae_false;
    }
    
    if (((buffer->buffer_len - buffer->buffer_off) < len) &&
        !sae_buffer_realloc(buffer, len))
    {
        return sae_false;
    }
    
    sae_memcpy(buffer->buffer + buffer->buffer_off, buf, len);
    
    buffer->buffer_off += len;
    
    return sae_true;
}

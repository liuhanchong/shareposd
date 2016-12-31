//
//  test_sae_buffer.h
//  shareposd
//
//  Created by liuhanchong on 2016/12/31.
//  Copyright © 2016年 liuhanchong. All rights reserved.
//

#ifndef test_sae_buffer_h
#define test_sae_buffer_h

sae_int_t main_buffer(sae_int_t argc, sae_char_t **argv)
{
    sae_buffer_t *buffer = sae_buffer_create();
    if (buffer)
    {
        printf("%s", sae_buffer_get(buffer));
        
        sae_buffer_add(buffer, "1234\n", 5);
        
        printf("%s\n-----------\n", sae_buffer_get(buffer));
        
        sae_file_t *f = sae_file_open("server.ini", SAE_FILE_READ);
        
        if (!sae_buffer_read_file(buffer, f->fd))
        {
            printf("read failed\n");
        }
        
        sae_file_close(f);
        
        printf("%s\n-----------\n", sae_buffer_get(buffer));
        
        sae_buffer_add(buffer, "3456\n", 5);
        
        printf("%s\n-----------\n", sae_buffer_get(buffer));
        
        sae_int_t count = 1;
        sae_char_t *line = sae_buffer_read_line(buffer);
        while (line)
        {
            printf("%d %s\n", count++, line);
            sae_buffer_read_line_free(line);
            line = sae_buffer_read_line(buffer);
        }
        
        while (sae_true)
        {
            if (!sae_buffer_add(buffer, "1234567\n", 8))
            {
                printf("max len is %ld\n", buffer->buffer_len);
                printf("%s------------------------\n", sae_buffer_get(buffer));
                break;
            }
        }
        
        count = 1;
        line = sae_buffer_read_line(buffer);
        while (line)
        {
            printf("%d %s\n", count++, line);
            sae_buffer_read_line_free(line);
            line = sae_buffer_read_line(buffer);
        }
        
        sae_buffer_destroy(buffer);
        printf("---------------------\nok\n");
    }
    return 0;
}

#endif /* test_sae_buffer_h */

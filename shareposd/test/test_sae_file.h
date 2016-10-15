//
//  test_sae_file.h
//  shareposd
//
//  Created by liuhanchong on 2016/10/2.
//  Copyright © 2016年 liuhanchong. All rights reserved.
//

#ifndef test_sae_file_h
#define test_sae_file_h

int main_file(int argc, char *const *argv)
{
    char read[10];
    int i = 0;
    sae_file_t *file = sae_null;
    if (sae_file_exist(".\\123.txt"))
    {
        sae_file_rm(".\\123.txt");
        
        return 0;
    }
    else
    {
        file = sae_file_open(".\\123.txt", SAE_FILE_WRITE | SAE_FILE_READ | SAE_FILE_AUTO_CRE);
        if (file)
        {
            printf("文件长度%ld\n", sae_file_len(file->fd));
            
            for (i = 0; i < 300000; i++)
            {
                if (sae_file_write(file->fd, "123456789000000", 15) <= 0)
                {
                    printf("写入失败\n");
                }
            }
            
            if (sae_file_read(file->fd, read, 9) < 0)
            {
                printf("读取失败\n");
            }
            read[9] = sae_str_end;
            
            printf("%s\n", read);
            
            printf("文件长度%ld\n", sae_file_len(file->fd));
            
            sae_file_close(file);
            
            return 0;
        }
        
        printf("文件不存在\r\n");
    }
    
    return 1;
}


#endif /* test_sae_file_h */

//
//  test_sae_log.h
//  shareposd
//
//  Created by liuhanchong on 2016/10/3.
//  Copyright © 2016年 liuhanchong. All rights reserved.
//

#ifndef test_sae_log_h
#define test_sae_log_h

int main_log(int argc, char *const *argv)
{
    int i = 0;
    sae_log_t *log = sae_log_create();
    if (!log)
    {
        printf("create log failed\n");
        return 1;
    }
    
    for (i = 0; i < 10000; i++)
    {
        sae_log_write(log, LDEBUG, "123%d%s", 4, "lhc_1");
        
        sae_log_write(log, LERROR, "123%d%s", 4, "lhc_2");
        
        sae_log_write(log, LDUMP, "123%d%s", 4, "lhc_3");
        
        sae_log_write(log, LOTHER, "123%d%s", 4, "lhc_4");
    }
    
    sae_log_destroy(log);
}

#endif /* test_sae_log_h */

//
//  test_sae_parse.h
//  shareposd
//
//  Created by liuhanchong on 2016/10/6.
//  Copyright © 2016年 liuhanchong. All rights reserved.
//

#ifndef test_sae_parse_h
#define test_sae_parse_h

sae_int_t main_parse(sae_int_t argc, sae_char_t **argv)
{
    int i = 0;
    sae_parse_t *parse = NULL;
    sae_char_t value[SAE_PARSE_ROW_MAX_LEN];
    
    sae_parse_char_t *section[10] = {"COMMUNICATION", "COMMUNICATION", "COMMUNICATION",
        "刘撼翀", "刘撼翀", "COMMUNICATION", "LOG", "刘撼翀", "COMMUNICATION", NULL};
    sae_parse_char_t *key[10] = {"123", "S_1", "12", "钱", "性a别", "钱", "TEST", "钱", "钱1", NULL};
    
    parse = sae_parse_create("/Users/liuhanchong/Desktop/server.ini");
    if (!parse)
    {
        printf("create failed");
        return 1;
    }
    
    for (i = 0; i < 6; i++)
    {
        if (sae_parse_string(parse, section[i], key[i], value, SAE_PARSE_ROW_MAX_LEN))
        {
            printf("%d %s \n", i + 1, value);
        }
        else
        {
            printf("error %d \n", i + 1);
        }
    }
    
    for (i = 6; i < 9; i++)
    {
        printf("%d \n", sae_parse_int(parse, section[i], key[i], -1));
    }
    
    sae_parse_destroy(parse);
    
    return 0;
}


#endif /* test_sae_parse_h */

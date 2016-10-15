//
//  test.h
//  shareposd
//
//  Created by liuhanchong on 2016/10/1.
//  Copyright © 2016年 liuhanchong. All rights reserved.
//

#ifndef test_h
#define test_h

int main_array(int argc, char *const *argv)
{
    sae_array_t *array = sae_array_create(10, sae_sizeof(int));
    if (!array)
    {
        return 1;
    }
    
    for (int i = 0; i < 10; i++)
    {
        int *elt = sae_array_push(array);
        *elt = i;
        printf("%p, %d\n", elt, *((int *)sae_array_get_elt(array, i)));
    }
    
    sae_array_destroy(array);
    
    return 0;
}

#endif /* test_h */

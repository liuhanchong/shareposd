//
//  test_sae_list.h
//  shareposd
//
//  Created by liuhanchong on 2016/10/3.
//  Copyright © 2016年 liuhanchong. All rights reserved.
//

#ifndef test_sae_list_h
#define test_sae_list_h

int main_list(int argc, char *const *argv)
{
    sae_list_t *list = sae_list_create();
    if (!list)
    {
        printf("create list failed\n");
        return 1;
    }
    
    if (sae_list_empty(list))
    {
        printf("empty list\n");
    }
    
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    sae_list_push(list, &array[0]);
    sae_list_push(list, &array[1]);
    sae_list_push(list, &array[2]);
    sae_list_push(list, &array[3]);
    
    int count = 0;
    sae_list_node_t *node = list->head;
    while (node)
    {
        count++;
        if (count == 2)
        {
            sae_list_rm(list, node);
            break;
        }
        node = node->next;
    }
    
    sae_list_rm(list, list->head);
    sae_list_rm(list, list->last);
    
    node = list->head;
    while (node)
    {
        printf("%d\n", *((int *)(node->data)));
        node = node->next;
    }
    
    if (!sae_list_empty(list))
    {
        printf("not empty list\n");
    }
    
    sae_list_destroy(list);
    
    return 0;
}


#endif /* test_sae_list_h */

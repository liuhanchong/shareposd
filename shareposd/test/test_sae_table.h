//
//  main_sae_table.h
//  shareposd
//
//  Created by liuhanchong on 2016/10/3.
//  Copyright © 2016年 liuhanchong. All rights reserved.
//

#ifndef main_sae_table_h
#define main_sae_table_h

int main_table(int argc, char *const *argv)
{
    sae_table_t *table = sae_table_create(10);
    if (!table)
    {
        printf("create failed\n");
        return 1;
    }
    
    char *key[10] = {"1", "2", "3", "4", "5",
        "6", "7", "8", "9", "10"};
    
    char *value[10] = {"1", "2", "3", "4", "5",
        "6", "7", "8", "9", "10"};
    
    sae_ulong_t lv[10] = {0};
    
    for (int i = 0; i < 10; i++)
    {
        lv[i] = sae_str_hash(key[i]) % 10;
        sae_table_push(table, key[i], value[i]);
    }
    
    printf("打印生成hash值 ");
    for (int i = 0; i < 10; i++)
    {
        printf("%ld ", lv[i]);
    }
    printf("\n");
    
    printf("打印占用的array index ");
    sae_list_t *list = sae_null;
    for (int i = 0; i < table->table->nalloc; i++)
    {
        list = *(sae_list_t **)sae_array_get(table->table, i);
        if (list)
        {
            printf("%d ", i);
        }
    }
    printf("\n");
    
    printf("打印插入结果\n");
    sae_list_node_t *node = sae_null;
    for (int i = 0; i < table->table->nalloc; i++)
    {
        list = *(sae_list_t **)sae_array_get(table->table, i);
        if (list)
        {
            printf("index = %d ", i);
            /*free elt*/
            node = list->head;
            while (node)
            {
                sae_table_elt_t *elt = node->data;
                printf("%s->>>%s ", (char *)elt->key, (char *)elt->value);
                node = node->next;
            }
            printf("\n");
        }
    }
    
    printf("get value\n");
    printf("%s ", (char *)sae_table_value(table, "1"));
    printf("%s ", (char *)sae_table_value(table, "3"));
    printf("%s ", (char *)sae_table_value(table, "5"));
    printf("%s ", (char *)sae_table_value(table, "9"));
    printf("%s ", (char *)sae_table_value(table, "10"));
    printf("%s\n", (char *)sae_table_value(table, "11"));
    
    sae_table_rm(table, "3");
    sae_table_rm(table, "10");
    sae_table_rm(table, "9");
    sae_table_rm(table, "2");
    
    printf("打印插入结果\n");
    for (int i = 0; i < table->table->nalloc; i++)
    {
        list = *(sae_list_t **)sae_array_get(table->table, i);
        if (list)
        {
            printf("index = %d ", i);
            /*free elt*/
            node = list->head;
            while (node)
            {
                sae_table_elt_t *elt = node->data;
                printf("%s->>>%s ", (char *)elt->key, (char *)elt->value);
                node = node->next;
            }
            printf("\n");
        }
    }
    
    sae_table_push(table, key[2], value[2]);
    sae_table_push(table, key[9], value[9]);
    
    printf("打印插入结果\n");
    for (int i = 0; i < table->table->nalloc; i++)
    {
        list = *(sae_list_t **)sae_array_get(table->table, i);
        if (list)
        {
            printf("index = %d ", i);
            /*free elt*/
            node = list->head;
            while (node)
            {
                sae_table_elt_t *elt = node->data;
                printf("%s->>>%s ", (char *)elt->key, (char *)elt->value);
                node = node->next;
            }
            printf("\n");
        }
    }
    
    sae_table_destroy(table);
    
    return 0;
}

#endif /* main_sae_table_h */

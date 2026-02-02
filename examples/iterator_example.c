/**
 * @file iterator_example.c
 * @brief Universal iterator example across all data structures
 * 
 * Demonstrates v1.0.0 features:
 * - Unified iterator API
 * - Iterating vectors, lists, and maps
 */

#define NANODS_IMPLEMENTATION
#include "../nanods.h"
#include <stdio.h>

int main(void) {
    printf("==============================================\n");
    printf("  Universal Iterator Example (NanoDS v%s)\n", NANODS_VERSION);
    printf("==============================================\n\n");
    
    nanods_seed_init(0);
    
    /* Example 1: Iterate Vector */
    {
        printf("Example 1: Vector Iterator\n");
        printf("─────────────────────────────────\n");
        
        IntVector vec;
        nv_init_int(&vec);
        
        for (int i = 1; i <= 5; i++) {
            nv_push_int(&vec, i * 10);
        }
        
        printf("Vector contents:  ");
        NanoIter it = nv_iter_int(&vec);
        while (! it.finished) {
            int* val = (int*)it.ptr;
            printf("%d ", *val);
            nv_iter_next_int(&it);
        }
        printf("\n\n");
        
        nv_free_int(&vec);
    }
    
    /* Example 2: Iterate List */
    {
        printf("Example 2: Singly Linked List Iterator\n");
        printf("─────────────────────────────────\n");
        
        IntList list;
        nl_init_int(&list);
        
        for (int i = 1; i <= 5; i++) {
            nl_push_back_int(&list, i * 100);
        }
        
        printf("List contents: ");
        NanoIter it = nl_iter_int(&list);
        while (!it.finished) {
            int* val = (int*)it.ptr;
            printf("%d ", *val);
            nl_iter_next_int(&it);
        }
        printf("\n\n");
        
        nl_free_int(&list);
    }
    
    /* Example 3: Iterate Doubly Linked List */
    {
        printf("Example 3: Doubly Linked List Iterator\n");
        printf("─────────────────────────────────\n");
        
        IntList2 list;
        nl2_init_int(&list);
        
        for (int i = 1; i <= 5; i++) {
            nl2_push_back_int(&list, i * 1000);
        }
        
        printf("List2 contents: ");
        NanoIter it = nl2_iter_int(&list);
        while (!it.finished) {
            int* val = (int*)it.ptr;
            printf("%d ", *val);
            nl2_iter_next_int(&it);
        }
        printf("\n\n");
        
        nl2_free_int(&list);
    }
    
    /* Example 4: Iterate Map */
    {
        printf("Example 4: Map Iterator\n");
        printf("─────────────────────────────────\n");
        
        NanoMap map;
        nm_init(&map);
        
        int val1 = 100, val2 = 200, val3 = 300;
        nm_set(&map, "apple", &val1);
        nm_set(&map, "banana", &val2);
        nm_set(&map, "cherry", &val3);
        
        printf("Map contents:\n");
        NanoMapIterator iter;
        nm_iter_init(&iter, &map);
        
        const char* key;
        void* value;
        while (nm_iter_next(&iter, &key, &value) == NANODS_OK) {
            printf("  %s:  %d\n", key, *(int*)value);
        }
        printf("\n");
        
        nm_free(&map);
    }
    
    printf("==============================================\n");
    printf("Universal iterators provide:\n");
    printf("  ✓ Consistent API across all containers\n");
    printf("  ✓ Simple for-each style iteration\n");
    printf("  ✓ Type-safe element access\n");
    printf("==============================================\n");
    
    return 0;
}
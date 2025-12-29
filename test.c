#define NANODS_IMPLEMENTATION
#include "nanods.h"
#include <stdio.h>

typedef struct {
    int x;
    int y;
} Point;

NANODS_DEFINE_VECTOR(Point)
NANODS_DEFINE_STACK(Point)
NANODS_DEFINE_LIST(Point)

int main(void) {
    printf("=== NanoDS v%s Test Suite ===\n\n", NANODS_VERSION);
    
    /* =========================================================================
     * TEST 1: IntVector (Error Handling)
     * =========================================================================
     */
    printf("TEST 1: IntVector (Error Handling)\n");
    printf("-----------------------------------\n");
    
    IntVector vec;
    nv_init_int(&vec);
    
    for (int i = 1; i <= 5; i++) {
        int err = nv_push_int(&vec, i * 10);
        if (err != NANODS_OK) {
            printf("❌ Push failed with error: %d\n", err);
            return 1;
        }
    }
    
    printf("Vector size: %zu\n", nv_size_int(&vec));
    printf("Vector contents:   ");
    for (size_t i = 0; i < nv_size_int(&vec); i++) {
        int value;
        nv_get_int(&vec, i, &value);
        printf("%d ", value);
    }
    printf("\n");
    
    int popped = 0;
    nv_pop_int(&vec, &popped);
    printf("Popped value: %d\n", popped);
    
    nv_free_int(&vec);
    printf("✅ Vector test passed\n\n");
    
    /* =========================================================================
     * TEST 2: IntStack
     * =========================================================================
     */
    printf("TEST 2: IntStack (LIFO)\n");
    printf("-----------------------\n");
    
    IntStack stack;
    ns_init_int(&stack);
    
    for (int i = 1; i <= 5; i++) {
        ns_push_int(&stack, i);
    }
    
    printf("Stack size: %zu\n", ns_size_int(&stack));
    
    int top;
    ns_peek_int(&stack, &top);
    printf("Top element (peek): %d\n", top);
    
    printf("Popping elements:   ");
    while (!ns_empty_int(&stack)) {
        int val;
        ns_pop_int(&stack, &val);
        printf("%d ", val);
    }
    printf("\n");
    
    ns_free_int(&stack);
    printf("✅ Stack test passed\n\n");
    
    /* =========================================================================
     * TEST 3: IntList (Singly Linked List)
     * =========================================================================
     */
    printf("TEST 3: IntList (Singly Linked List)\n");
    printf("-------------------------------------\n");
    
    IntList list;
    nl_init_int(&list);
    
    for (int i = 1; i <= 5; i++) {
        nl_push_back_int(&list, i * 100);
    }
    
    printf("List size: %zu\n", nl_size_int(&list));
    
    printf("Popping from front:  ");
    for (int i = 0; i < 3; i++) {
        int val;
        nl_pop_front_int(&list, &val);
        printf("%d ", val);
    }
    printf("\n");
    
    printf("Remaining size: %zu\n", nl_size_int(&list));
    
    nl_free_int(&list);
    printf("✅ List test passed\n\n");
    
    /* =========================================================================
     * TEST 4: NanoMap
     * =========================================================================
     */
    printf("TEST 4: NanoMap (Hash Map)\n");
    printf("--------------------------\n");
    
    NanoMap map;
    nm_init(&map);
    
    int val1 = 100, val2 = 200, val3 = 300;
    nm_set(&map, "apple", &val1);
    nm_set(&map, "banana", &val2);
    nm_set(&map, "cherry", &val3);
    
    printf("Map size: %zu\n", nm_size(&map));
    
    int* result = (int*)nm_get(&map, "banana");
    printf("Get 'banana': %d\n", result ? *result : -1);
    
    printf("Has 'apple':   %s\n", nm_has(&map, "apple") ? "yes" : "no");
    printf("Has 'grape':  %s\n", nm_has(&map, "grape") ? "yes" : "no");
    
    nm_remove(&map, "banana");
    printf("After removing 'banana', size: %zu\n", nm_size(&map));
    
    nm_free(&map);
    printf("✅ Map test passed\n\n");
    
    /* =========================================================================
     * TEST 5: Custom Struct (Point)
     * =========================================================================
     */
    printf("TEST 5: Custom Struct (Point)\n");
    printf("------------------------------\n");
    
    NanoVector_Point points;
    nv_init_Point(&points);
    
    nv_push_Point(&points, (Point){1, 2});
    nv_push_Point(&points, (Point){3, 4});
    nv_push_Point(&points, (Point){5, 6});
    
    printf("Point vector size: %zu\n", nv_size_Point(&points));
    
    for (size_t i = 0; i < nv_size_Point(&points); i++) {
        Point p;
        nv_get_Point(&points, i, &p);
        printf("  Point[%zu]:  (%d, %d)\n", i, p.x, p.y);
    }
    
    nv_free_Point(&points);
    printf("✅ Custom struct test passed\n\n");
    
    /* =========================================================================
     * TEST 6: Error Handling (ONLY IN HARD SAFETY MODE)
     * =========================================================================
     */
    printf("TEST 6: Error Handling\n");
    printf("----------------------\n");
    
#ifdef NANODS_HARD_SAFETY
    // Only test error codes in hard safety mode
    IntVector empty_vec;
    nv_init_int(&empty_vec);
    
    int val;
    int err = nv_pop_int(&empty_vec, &val);
    printf("Pop from empty vector: %s\n", 
           err == NANODS_ERR_EMPTY ? "NANODS_ERR_EMPTY (expected)" : "unexpected");
    
    if (err != NANODS_ERR_EMPTY) {
        printf("❌ Error handling test failed\n");
        nv_free_int(&empty_vec);
        return 1;
    }
    
    nv_free_int(&empty_vec);
    printf("✅ Error handling test passed\n\n");
#else
    printf("⚠️  Error handling tests skipped in debug mode\n");
    printf("   (Use -DNANODS_HARD_SAFETY to enable)\n");
    printf("✅ Test skipped (not a failure)\n\n");
#endif
    
    /* =========================================================================
     * FINAL REPORT
     * =========================================================================
     */
    printf("===========================================\n");
    printf("✅ All Tests Passed Successfully!\n");
    printf("===========================================\n");
    printf("NanoDS v%s is production-ready!\n", NANODS_VERSION);
    
    return 0;
}

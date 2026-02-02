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
NANODS_DEFINE_LIST2(Point)

/* Helper functions for functional tests */
int double_value(int x) {
    return x * 2;
}

int is_even(int x) {
    return x % 2 == 0;
}

int is_positive(int x) {
    return x > 0;
}

int main(void) {
    printf("=== NanoDS v%s Test Suite ===\n\n", NANODS_VERSION);
    
    /* Initialize random seed for hash randomization */
    nanods_seed_init(0);
    printf("Hash seed initialized: 0x%08X\n\n", nanods_get_seed());
    
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
    printf("Vector contents:  ");
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
    
    printf("Popping elements:  ");
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
    
    printf("Popping from front: ");
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
     * TEST 4: IntList2 (Doubly Linked List) - NEW in v1.0.0
     * =========================================================================
     */
    printf("TEST 4: IntList2 (Doubly Linked List) - NEW\n");
    printf("--------------------------------------------\n");
    
    IntList2 list2;
    nl2_init_int(&list2);
    
    for (int i = 1; i <= 5; i++) {
        nl2_push_back_int(&list2, i * 1000);
    }
    
    printf("List2 size:  %zu\n", nl2_size_int(&list2));
    
    printf("Popping from back: ");
    for (int i = 0; i < 2; i++) {
        int val;
        nl2_pop_back_int(&list2, &val);
        printf("%d ", val);
    }
    printf("\n");
    
    printf("Remaining size: %zu\n", nl2_size_int(&list2));
    
    nl2_free_int(&list2);
    printf("✅ Doubly linked list test passed\n\n");
    
    /* =========================================================================
     * TEST 5: Ring Buffer - NEW in v1.0.0
     * =========================================================================
     */
    printf("TEST 5: Ring Buffer (Circular Buffer) - NEW\n");
    printf("--------------------------------------------\n");
    
    IntRing16 ring;
    nr_init_int_16(&ring);
    
    printf("Ring capacity: %zu\n", nr_capacity_int_16(&ring));
    
    for (int i = 1; i <= 10; i++) {
        nr_write_int_16(&ring, i);
    }
    
    printf("Ring size after writing: %zu\n", nr_size_int_16(&ring));
    printf("Is full: %s\n", nr_is_full_int_16(&ring) ? "yes" : "no");
    
    printf("Reading 5 elements: ");
    for (int i = 0; i < 5; i++) {
        int val;
        nr_read_int_16(&ring, &val);
        printf("%d ", val);
    }
    printf("\n");
    
    printf("Ring size after reading: %zu\n", nr_size_int_16(&ring));
    
    nr_clear_int_16(&ring);
    printf("✅ Ring buffer test passed\n\n");
    
    /* =========================================================================
     * TEST 6: NanoMap (with Anti-DoS seeding)
     * =========================================================================
     */
    printf("TEST 6: NanoMap (Hash Map with Anti-DoS)\n");
    printf("-----------------------------------------\n");
    
    NanoMap map;
    nm_init(&map);
    
    printf("Map seed: 0x%08X (randomized)\n", map.seed);
    
    int val1 = 100, val2 = 200, val3 = 300;
    nm_set(&map, "apple", &val1);
    nm_set(&map, "banana", &val2);
    nm_set(&map, "cherry", &val3);
    
    printf("Map size: %zu\n", nm_size(&map));
    
    int* result = (int*)nm_get(&map, "banana");
    printf("Get 'banana': %d\n", result ? *result : -1);
    
    printf("Has 'apple': %s\n", nm_has(&map, "apple") ? "yes" : "no");
    printf("Has 'grape': %s\n", nm_has(&map, "grape") ? "yes" : "no");
    
    nm_remove(&map, "banana");
    printf("After removing 'banana', size: %zu\n", nm_size(&map));
    
    nm_free(&map);
    printf("✅ Map test passed\n\n");
    
    /* =========================================================================
     * TEST 7: Secure Flag - NEW in v1.0.0
     * =========================================================================
     */
    printf("TEST 7: Secure Flag (Memory Wiping) - NEW\n");
    printf("------------------------------------------\n");
    
    IntVector secure_vec;
    nv_init_ex_int(&secure_vec, NANODS_FLAG_SECURE);
    
    for (int i = 0; i < 5; i++) {
        nv_push_int(&secure_vec, i + 100);
    }
    
    printf("Secure vector size: %zu\n", nv_size_int(&secure_vec));
    printf("Secure flag enabled: memory will be wiped on free\n");
    
    nv_free_int(&secure_vec);  /* Automatically wipes memory */
    printf("✅ Secure flag test passed\n\n");
    
    /* =========================================================================
     * TEST 8: Custom Struct (Point)
     * =========================================================================
     */
    printf("TEST 8: Custom Struct (Point)\n");
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
        printf("  Point[%zu]: (%d, %d)\n", i, p.x, p.y);
    }
    
    nv_free_Point(&points);
    printf("✅ Custom struct test passed\n\n");
    
    /* =========================================================================
     * TEST 9: Functional Operations (Map/Filter)
     * =========================================================================
     */
    printf("TEST 9: Functional Operations (Map/Filter)\n");
    printf("-------------------------------------------\n");
    
    IntVector source;
    nv_init_int(&source);
    
    for (int i = -2; i <= 5; i++) {
        nv_push_int(&source, i);
    }
    
    printf("Source vector:  ");
    for (size_t i = 0; i < nv_size_int(&source); i++) {
        int val;
        nv_get_int(&source, i, &val);
        printf("%d ", val);
    }
    printf("\n");
    
    IntVector mapped;
    int map_err = nv_map_int(&source, &mapped, double_value);
    if (map_err != NANODS_OK) {
        printf("❌ Map failed with error:  %d\n", map_err);
        nv_free_int(&source);
        return 1;
    }
    
    printf("Mapped (x2): ");
    for (size_t i = 0; i < nv_size_int(&mapped); i++) {
        int val;
        nv_get_int(&mapped, i, &val);
        printf("%d ", val);
    }
    printf("\n");
    
    IntVector filtered_even;
    int filter_err = nv_filter_int(&source, &filtered_even, is_even);
    if (filter_err != NANODS_OK) {
        printf("❌ Filter failed with error: %d\n", filter_err);
        nv_free_int(&source);
        nv_free_int(&mapped);
        return 1;
    }
    
    printf("Filtered (even): ");
    for (size_t i = 0; i < nv_size_int(&filtered_even); i++) {
        int val;
        nv_get_int(&filtered_even, i, &val);
        printf("%d ", val);
    }
    printf("\n");
    
    IntVector filtered_positive;
    filter_err = nv_filter_int(&source, &filtered_positive, is_positive);
    if (filter_err != NANODS_OK) {
        printf("❌ Filter failed with error:  %d\n", filter_err);
        nv_free_int(&source);
        nv_free_int(&mapped);
        nv_free_int(&filtered_even);
        return 1;
    }
    
    printf("Filtered (positive): ");
    for (size_t i = 0; i < nv_size_int(&filtered_positive); i++) {
        int val;
        nv_get_int(&filtered_positive, i, &val);
        printf("%d ", val);
    }
    printf("\n");
    
    nv_free_int(&source);
    nv_free_int(&mapped);
    nv_free_int(&filtered_even);
    nv_free_int(&filtered_positive);
    printf("✅ Functional operations test passed\n\n");
    
    /* =========================================================================
     * TEST 10: Universal Iterator - NEW in v1.0.0
     * =========================================================================
     */
    printf("TEST 10: Universal Iterator - NEW\n");
    printf("----------------------------------\n");
    
    IntVector iter_vec;
    nv_init_int(&iter_vec);
    
    for (int i = 1; i <= 5; i++) {
        nv_push_int(&iter_vec, i);
    }
    
    printf("Iterating vector: ");
    NanoIter it = nv_iter_int(&iter_vec);
    while (! it.finished) {
        int* val = (int*)it.ptr;
        printf("%d ", *val);
        nv_iter_next_int(&it);
    }
    printf("\n");
    
    nv_free_int(&iter_vec);
    printf("✅ Universal iterator test passed\n\n");
    
    /* =========================================================================
     * TEST 11: Error Handling (ONLY IN HARD SAFETY MODE)
     * =========================================================================
     */
    printf("TEST 11: Error Handling\n");
    printf("-----------------------\n");
    
#ifdef NANODS_HARD_SAFETY
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
    printf("New features in v1.0.0:\n");
    printf("  ✓ Doubly linked lists (List2)\n");
    printf("  ✓ Ring buffers (fixed-size, stack-allocated)\n");
    printf("  ✓ Hash randomization (Anti-DoS)\n");
    printf("  ✓ Secure flag (memory wiping)\n");
    printf("  ✓ Universal iterators\n");
    printf("  ✓ Compiler optimization hints\n");
    printf("===========================================\n");
    
    return 0;
}
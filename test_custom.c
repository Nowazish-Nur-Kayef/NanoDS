#define NANODS_IMPLEMENTATION
#include "nanods.h"
#include <stdio.h>

int main(void) {
    printf("=== My Custom NanoDS Test ===\n\n");
    
    // Test 1: Integer vector
    IntVector numbers;
    nv_init_int(&numbers);
    
    for (int i = 1; i <= 5; i++) {
        nv_push_int(&numbers, i * 10);
    }
    
    printf("Numbers:  ");
    for (size_t i = 0; i < nv_size_int(&numbers); i++) {
        printf("%d ", nv_get_int(&numbers, i));
    }
    printf("\n");
    
    nv_free_int(&numbers);
    
    // Test 2: Hash map
    NanoMap config;
    nm_init(&config);
    
    int port = 8080;
    nm_set(&config, "port", &port);
    
    int* result = (int*)nm_get(&config, "port");
    printf("Port: %d\n", result ? *result : -1);
    
    nm_free(&config);
    
    printf("\n✅ Done!\n");
    return 0;
}

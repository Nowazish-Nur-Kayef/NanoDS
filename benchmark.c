#if defined(__linux__) || defined(__unix__)
    #ifndef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE 199309L
    #endif
#endif

#define NANODS_IMPLEMENTATION
#include "nanods.h"
#include <stdio.h>
#include <time.h>

#define BENCHMARK_ITERATIONS 1000000

/* Cross-platform time measurement */
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    
    double get_time_ms(void) {
        LARGE_INTEGER frequency;
        LARGE_INTEGER counter;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&counter);
        return (double)(counter.QuadPart * 1000.0) / frequency.QuadPart;
    }
#elif defined(__APPLE__) || defined(__MACH__)
    #include <mach/mach_time.h>
    
    double get_time_ms(void) {
        static mach_timebase_info_data_t timebase;
        if (timebase.denom == 0) {
            mach_timebase_info(&timebase);
        }
        uint64_t time = mach_absolute_time();
        return (double)(time * timebase.numer / timebase. denom) / 1000000.0;
    }
#else
    /* Linux and POSIX systems */
    double get_time_ms(void) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
    }
#endif

void benchmark_vector_push(void) {
    printf("🚀 Benchmarking:  NanoVector Push Operations\n");
    printf("   Iterations:  %d\n", BENCHMARK_ITERATIONS);
    
    IntVector vec;
    nv_init_int(&vec);
    
    double start = get_time_ms();
    
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
        nv_push_int(&vec, i);
    }
    
    double end = get_time_ms();
    double elapsed = end - start;
    double ops_per_sec = BENCHMARK_ITERATIONS / (elapsed / 1000.0);
    
    printf("   Time:        %.2f ms\n", elapsed);
    printf("   Ops/sec:     %.0f\n", ops_per_sec);
    printf("   Per-op:      %.3f us\n\n", (elapsed * 1000.0) / BENCHMARK_ITERATIONS);
    
    nv_free_int(&vec);
}

void benchmark_stack_push_pop(void) {
    printf("🚀 Benchmarking: NanoStack Push/Pop Operations\n");
    printf("   Iterations:  %d\n", BENCHMARK_ITERATIONS);
    
    IntStack stack;
    ns_init_int(&stack);
    
    double start = get_time_ms();
    
    for (int i = 0; i < BENCHMARK_ITERATIONS / 2; i++) {
        ns_push_int(&stack, i);
    }
    
    int value;
    for (int i = 0; i < BENCHMARK_ITERATIONS / 2; i++) {
        ns_pop_int(&stack, &value);
    }
    
    double end = get_time_ms();
    double elapsed = end - start;
    double ops_per_sec = BENCHMARK_ITERATIONS / (elapsed / 1000.0);
    
    printf("   Time:        %.2f ms\n", elapsed);
    printf("   Ops/sec:     %.0f\n", ops_per_sec);
    printf("   Per-op:      %.3f us\n\n", (elapsed * 1000.0) / BENCHMARK_ITERATIONS);
    
    ns_free_int(&stack);
}

void benchmark_list_operations(void) {
    printf("🚀 Benchmarking: NanoList Push Operations\n");
    printf("   Iterations: %d\n", BENCHMARK_ITERATIONS / 10);
    
    IntList list;
    nl_init_int(&list);
    
    double start = get_time_ms();
    
    for (int i = 0; i < BENCHMARK_ITERATIONS / 10; i++) {
        nl_push_back_int(&list, i);
    }
    
    double end = get_time_ms();
    double elapsed = end - start;
    double ops_per_sec = (BENCHMARK_ITERATIONS / 10) / (elapsed / 1000.0);
    
    printf("   Time:        %.2f ms\n", elapsed);
    printf("   Ops/sec:     %.0f\n", ops_per_sec);
    printf("   Per-op:      %.3f us\n\n", (elapsed * 1000.0) / (BENCHMARK_ITERATIONS / 10));
    
    nl_free_int(&list);
}

void benchmark_hashmap_operations(void) {
    printf("🚀 Benchmarking: NanoMap Set/Get Operations\n");
    printf("   Iterations: %d\n", BENCHMARK_ITERATIONS / 100);
    
    NanoMap map;
    nm_init(&map);
    
    int values[BENCHMARK_ITERATIONS / 100];
    char keys[BENCHMARK_ITERATIONS / 100][32];
    
    for (int i = 0; i < BENCHMARK_ITERATIONS / 100; i++) {
        snprintf(keys[i], 32, "key_%d", i);
        values[i] = i;
    }
    
    double start = get_time_ms();
    
    for (int i = 0; i < BENCHMARK_ITERATIONS / 100; i++) {
        nm_set(&map, keys[i], &values[i]);
    }
    
    for (int i = 0; i < BENCHMARK_ITERATIONS / 100; i++) {
        nm_get(&map, keys[i]);
    }
    
    double end = get_time_ms();
    double elapsed = end - start;
    double ops_per_sec = (BENCHMARK_ITERATIONS / 50) / (elapsed / 1000.0);
    
    printf("   Time:        %. 2f ms\n", elapsed);
    printf("   Ops/sec:     %.0f\n", ops_per_sec);
    printf("   Per-op:       %.3f us\n\n", (elapsed * 1000.0) / (BENCHMARK_ITERATIONS / 50));
    
    nm_free(&map);
}

int main(void) {
    printf("==============================================\n");
    printf("  NanoDS Performance Benchmark Suite v0.1.0\n");
    printf("==============================================\n\n");
    
    benchmark_vector_push();
    benchmark_stack_push_pop();
    benchmark_list_operations();
    benchmark_hashmap_operations();
    
    printf("==============================================\n");
    printf("✅ Benchmark Complete\n");
    printf("==============================================\n");
    
    return 0;
}

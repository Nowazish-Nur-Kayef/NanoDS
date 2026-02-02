#if defined(_POSIX_C_SOURCE) || defined(__linux__) || defined(__unix__)
    #ifndef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE 199309L
    #endif
#endif

#define NANODS_IMPLEMENTATION
#include "../nanods.h"
#include <stdio.h>
#include <time.h>

#define ITERATIONS 100000

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    double get_time_ms(void) {
        LARGE_INTEGER frequency, counter;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&counter);
        return (double)(counter.QuadPart * 1000.0) / frequency.QuadPart;
    }
#elif defined(__APPLE__) || defined(__MACH__)
    #include <mach/mach_time.h>
    double get_time_ms(void) {
        static mach_timebase_info_data_t timebase;
        if (timebase.denom == 0) mach_timebase_info(&timebase);
        uint64_t time = mach_absolute_time();
        return (double)(time * timebase.numer / timebase.denom) / 1000000.0;
    }
#else
    double get_time_ms(void) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts. tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
    }
#endif

int main(void) {
    printf("==============================================\n");
    printf("  NanoDS v%s Doubly Linked List Benchmark\n", NANODS_VERSION);
    printf("==============================================\n\n");
    
    nanods_seed_init(0);
    
    /* Benchmark 1: Push Front */
    {
        IntList2 list;
        nl2_init_int(&list);
        
        double start = get_time_ms();
        for (int i = 0; i < ITERATIONS; i++) {
            nl2_push_front_int(&list, i);
        }
        double end = get_time_ms();
        double elapsed = end - start;
        
        printf("Push Front (%d ops):\n", ITERATIONS);
        printf("  Time:       %.2f ms\n", elapsed);
        printf("  Throughput: %.0f ops/sec\n", ITERATIONS / (elapsed / 1000.0));
        printf("  Latency:     %.3f µs/op\n\n", (elapsed * 1000.0) / ITERATIONS);
        
        nl2_free_int(&list);
    }
    
    /* Benchmark 2: Push Back */
    {
        IntList2 list;
        nl2_init_int(&list);
        
        double start = get_time_ms();
        for (int i = 0; i < ITERATIONS; i++) {
            nl2_push_back_int(&list, i);
        }
        double end = get_time_ms();
        double elapsed = end - start;
        
        printf("Push Back (%d ops):\n", ITERATIONS);
        printf("  Time:       %.2f ms\n", elapsed);
        printf("  Throughput: %.0f ops/sec\n", ITERATIONS / (elapsed / 1000.0));
        printf("  Latency:    %.3f µs/op\n\n", (elapsed * 1000.0) / ITERATIONS);
        
        nl2_free_int(&list);
    }
    
    /* Benchmark 3: Pop Front */
    {
        IntList2 list;
        nl2_init_int(&list);
        
        for (int i = 0; i < ITERATIONS; i++) {
            nl2_push_back_int(&list, i);
        }
        
        double start = get_time_ms();
        int val;
        for (int i = 0; i < ITERATIONS; i++) {
            nl2_pop_front_int(&list, &val);
        }
        double end = get_time_ms();
        double elapsed = end - start;
        
        printf("Pop Front (%d ops):\n", ITERATIONS);
        printf("  Time:       %.2f ms\n", elapsed);
        printf("  Throughput: %.0f ops/sec\n", ITERATIONS / (elapsed / 1000.0));
        printf("  Latency:    %.3f µs/op\n\n", (elapsed * 1000.0) / ITERATIONS);
        
        nl2_free_int(&list);
    }
    
    /* Benchmark 4: Pop Back */
    {
        IntList2 list;
        nl2_init_int(&list);
        
        for (int i = 0; i < ITERATIONS; i++) {
            nl2_push_back_int(&list, i);
        }
        
        double start = get_time_ms();
        int val;
        for (int i = 0; i < ITERATIONS; i++) {
            nl2_pop_back_int(&list, &val);
        }
        double end = get_time_ms();
        double elapsed = end - start;
        
        printf("Pop Back (%d ops):\n", ITERATIONS);
        printf("  Time:       %.2f ms\n", elapsed);
        printf("  Throughput: %.0f ops/sec\n", ITERATIONS / (elapsed / 1000.0));
        printf("  Latency:     %.3f µs/op\n\n", (elapsed * 1000.0) / ITERATIONS);
        
        nl2_free_int(&list);
    }
    
    printf("==============================================\n");
    return 0;
}
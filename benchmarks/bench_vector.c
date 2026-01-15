#if defined(_POSIX_C_SOURCE) || defined(__linux__) || defined(__unix__)
    #ifndef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE 199309L
    #endif
#endif

#define NANODS_IMPLEMENTATION
#include "../nanods.h"
#include <stdio.h>
#include <time.h>

#define ITERATIONS 1000000

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
        return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
    }
#endif

int main(void) {
    printf("==============================================\n");
    printf("  NanoDS Vector Benchmark\n");
    printf("==============================================\n\n");
    
    /* Benchmark 1: Sequential Push */
    {
        IntVector vec;
        nv_init_int(&vec);
        
        double start = get_time_ms();
        for (int i = 0; i < ITERATIONS; i++) {
            nv_push_int(&vec, i);
        }
        double end = get_time_ms();
        double elapsed = end - start;
        
        printf("Sequential Push (%d ops):\n", ITERATIONS);
        printf("  Time:         %.2f ms\n", elapsed);
        printf("  Throughput:   %.0f ops/sec\n", ITERATIONS / (elapsed / 1000.0));
        printf("  Latency:     %.3f µs/op\n\n", (elapsed * 1000.0) / ITERATIONS);
        
        nv_free_int(&vec);
    }
    
    /* Benchmark 2: Random Access (Get) */
    {
        IntVector vec;
        nv_init_int(&vec);
        
        for (int i = 0; i < 10000; i++) {
            nv_push_int(&vec, i);
        }
        
        double start = get_time_ms();
        int val;
        for (int i = 0; i < ITERATIONS; i++) {
            nv_get_int(&vec, i % 10000, &val);
        }
        double end = get_time_ms();
        double elapsed = end - start;
        
        printf("Random Access Get (%d ops):\n", ITERATIONS);
        printf("  Time:        %.2f ms\n", elapsed);
        printf("  Throughput:  %.0f ops/sec\n", ITERATIONS / (elapsed / 1000.0));
        printf("  Latency:     %.3f µs/op\n\n", (elapsed * 1000.0) / ITERATIONS);
        
        nv_free_int(&vec);
    }
    
    /* Benchmark 3: Reserve Performance */
    {
        IntVector vec;
        nv_init_int(&vec);
        
        double start = get_time_ms();
        nv_reserve_int(&vec, ITERATIONS);
        for (int i = 0; i < ITERATIONS; i++) {
            nv_push_int(&vec, i);
        }
        double end = get_time_ms();
        double elapsed = end - start;
        
        printf("Pre-Reserved Push (%d ops):\n", ITERATIONS);
        printf("  Time:        %.2f ms\n", elapsed);
        printf("  Throughput:  %.0f ops/sec\n", ITERATIONS / (elapsed / 1000.0));
        printf("  Latency:     %.3f µs/op\n\n", (elapsed * 1000.0) / ITERATIONS);
        
        nv_free_int(&vec);
    }
    
    printf("==============================================\n");
    return 0;
}

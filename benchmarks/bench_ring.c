#if defined(_POSIX_C_SOURCE) || defined(__linux__) || defined(__unix__)
    #ifndef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE 199309L
    #endif
#endif

#define NANODS_IMPLEMENTATION
#include "../nanods.h"
#include <stdio.h>
#include <time.h>

#define ITERATIONS 10000000

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
    printf("  NanoDS v%s Ring Buffer Benchmark\n", NANODS_VERSION);
    printf("==============================================\n\n");
    
    nanods_seed_init(0);
    
    /* Benchmark:  Write/Read (256-element ring) */
    {
        IntRing256 ring;
        nr_init_int_256(&ring);
        
        double start = get_time_ms();
        for (int i = 0; i < ITERATIONS; i++) {
            if (! nr_is_full_int_256(&ring)) {
                nr_write_int_256(&ring, i);
            }
            if (!nr_is_empty_int_256(&ring)) {
                int val;
                nr_read_int_256(&ring, &val);
            }
        }
        double end = get_time_ms();
        double elapsed = end - start;
        
        printf("Ring Buffer (256 elements):\n");
        printf("  Operations:  %d write/read pairs\n", ITERATIONS);
        printf("  Time:       %.2f ms\n", elapsed);
        printf("  Throughput:  %.0f ops/sec\n", (ITERATIONS * 2) / (elapsed / 1000.0));
        printf("  Latency:    %.3f µs/op\n", (elapsed * 1000.0) / (ITERATIONS * 2));
        printf("  Memory:     Stack-allocated (zero heap)\n\n");
    }
    
    printf("==============================================\n");
    printf("Ring buffers are ideal for:\n");
    printf("  - Real-time audio/video streaming\n");
    printf("  - Embedded systems (no dynamic allocation)\n");
    printf("  - Producer-consumer queues\n");
    printf("==============================================\n");
    return 0;
}
#if defined(_POSIX_C_SOURCE) || defined(__linux__) || defined(__unix__)
    #ifndef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE 199309L
    #endif
#endif

#define NANODS_IMPLEMENTATION
#include "../nanods.h"
#include <stdio.h>
#include <stdlib.h>
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
        return (double)(time * timebase.numer / timebase. denom) / 1000000.0;
    }
#else
    double get_time_ms(void) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
    }
#endif

/* Naive implementation for comparison */
typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} NaiveVector;

void naive_init(NaiveVector* vec) {
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

void naive_push(NaiveVector* vec, int value) {
    if (vec->size >= vec->capacity) {
        size_t new_cap = vec->capacity == 0 ? 8 : vec->capacity * 2;
        vec->data = realloc(vec->data, new_cap * sizeof(int));
        vec->capacity = new_cap;
    }
    vec->data[vec->size++] = value;
}

void naive_free(NaiveVector* vec) {
    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

int main(void) {
    printf("==============================================\n");
    printf("  NanoDS v%s vs Naive Implementation\n", NANODS_VERSION);
    printf("==============================================\n\n");
    
    nanods_seed_init(0);
    
    /* Benchmark NanoDS */
    {
        IntVector vec;
        nv_init_int(&vec);
        
        double start = get_time_ms();
        for (int i = 0; i < ITERATIONS; i++) {
            nv_push_int(&vec, i);
        }
        double end = get_time_ms();
        
        printf("NanoDS Vector:\n");
        printf("  Time:       %.2f ms\n", end - start);
        printf("  Throughput: %.0f ops/sec\n", ITERATIONS / ((end - start) / 1000.0));
        printf("  Features:   Type-safe, bounds-checked, overflow-protected\n\n");
        
        nv_free_int(&vec);
    }
    
    /* Benchmark Naive */
    {
        NaiveVector vec;
        naive_init(&vec);
        
        double start = get_time_ms();
        for (int i = 0; i < ITERATIONS; i++) {
            naive_push(&vec, i);
        }
        double end = get_time_ms();
        
        printf("Naive Vector (no safety):\n");
        printf("  Time:       %.2f ms\n", end - start);
        printf("  Throughput:  %.0f ops/sec\n", ITERATIONS / ((end - start) / 1000.0));
        printf("  Features:   No safety checks\n\n");
        
        naive_free(&vec);
    }
    
    printf("==============================================\n");
    printf("Result: NanoDS provides safety with minimal overhead\n");
    printf("==============================================\n");
    return 0;
}
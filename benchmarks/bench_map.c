#if defined(_POSIX_C_SOURCE) || defined(__linux__) || defined(__unix__)
    #ifndef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE 199309L
    #endif
#endif

#define NANODS_IMPLEMENTATION
#include "../nanods.h"
#include <stdio.h>
#include <time.h>

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

void benchmark_map(int size) {
    NanoMap map;
    nm_init(&map);
    
    /* Prepare keys and values */
    char** keys = malloc(size * sizeof(char*));
    int* values = malloc(size * sizeof(int));
    
    for (int i = 0; i < size; i++) {
        keys[i] = malloc(32);
        snprintf(keys[i], 32, "key_%d", i);
        values[i] = i;
    }
    
    /* Benchmark:  Set */
    double start = get_time_ms();
    for (int i = 0; i < size; i++) {
        nm_set(&map, keys[i], &values[i]);
    }
    double end = get_time_ms();
    double set_time = end - start;
    
    /* Benchmark: Get */
    start = get_time_ms();
    for (int i = 0; i < size; i++) {
        nm_get(&map, keys[i]);
    }
    end = get_time_ms();
    double get_time = end - start;
    
    /* Benchmark:  Has */
    start = get_time_ms();
    for (int i = 0; i < size; i++) {
        nm_has(&map, keys[i]);
    }
    end = get_time_ms();
    double has_time = end - start;
    
    printf("Map Size: %d entries\n", size);
    printf("  Set:   %.2f ms (%.0f ops/sec)\n", set_time, size / (set_time / 1000.0));
    printf("  Get:  %.2f ms (%.0f ops/sec)\n", get_time, size / (get_time / 1000.0));
    printf("  Has:  %.2f ms (%.0f ops/sec)\n", has_time, size / (has_time / 1000.0));
    printf("  Load Factor: %.2f\n", (double)map.size / map.bucket_count);
    printf("  Seed: 0x%08X (Anti-DoS enabled)\n\n", map.seed);
    
    /* Cleanup */
    nm_free(&map);
    for (int i = 0; i < size; i++) {
        free(keys[i]);
    }
    free(keys);
    free(values);
}

int main(void) {
    printf("==============================================\n");
    printf("  NanoDS v%s Map Benchmark\n", NANODS_VERSION);
    printf("==============================================\n\n");
    
    /* Initialize random seed for hash randomization */
    nanods_seed_init(0);
    printf("Hash seed initialized:  0x%08X\n\n", nanods_get_seed());
    
    benchmark_map(1000);
    benchmark_map(10000);
    benchmark_map(100000);
    benchmark_map(1000000);
    
    printf("==============================================\n");
    return 0;
}
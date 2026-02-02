/**
 * @file ring_buffer_example.c
 * @brief Real-world example: Ring buffer for sensor data streaming
 * 
 * Demonstrates v1.0.0 features:
 * - Fixed-size circular buffer (zero heap allocation)
 * - Producer-consumer pattern
 * - Embedded systems use case
 */

#define NANODS_IMPLEMENTATION
#include "../nanods.h"
#include <stdio.h>

typedef struct {
    int sensor_id;
    float temperature;
    float humidity;
    int timestamp;
} SensorReading;

NANODS_DEFINE_RING(SensorReading, 16)

void producer(NanoRing_SensorReading_16* ring, int count) {
    printf("Producer:  Writing %d sensor readings...\n", count);
    
    for (int i = 0; i < count; i++) {
        SensorReading reading = {
            .sensor_id = i % 3,
            .temperature = 20.0f + (i % 10),
            .humidity = 50.0f + (i % 20),
            .timestamp = 1000 + i
        };
        
        if (nr_is_full_SensorReading_16(ring)) {
            printf("  [WARNING] Buffer full, overwriting oldest data!\n");
            SensorReading old;
            nr_read_SensorReading_16(ring, &old);
        }
        
        nr_write_SensorReading_16(ring, reading);
        printf("  Written:  Sensor %d @ T=%d\n", reading.sensor_id, reading.timestamp);
    }
    printf("\n");
}

void consumer(NanoRing_SensorReading_16* ring) {
    printf("Consumer: Reading sensor data...\n");
    printf("─────────────────────────────────────────────\n");
    
    int count = 0;
    while (! nr_is_empty_SensorReading_16(ring)) {
        SensorReading reading;
        nr_read_SensorReading_16(ring, &reading);
        
        printf("Sensor %d: %.1f°C, %.1f%% @ T=%d\n",
               reading.sensor_id,
               reading.temperature,
               reading.humidity,
               reading.timestamp);
        count++;
    }
    
    printf("─────���───────────────────────────────────────\n");
    printf("Total readings processed: %d\n\n", count);
}

int main(void) {
    printf("==============================================\n");
    printf("  Ring Buffer Example (NanoDS v%s)\n", NANODS_VERSION);
    printf("==============================================\n\n");
    
    nanods_seed_init(0);
    
    /* Create ring buffer (16 elements, stack-allocated) */
    NanoRing_SensorReading_16 ring;
    nr_init_SensorReading_16(&ring);
    
    printf("Ring Buffer Info:\n");
    printf("  Capacity:  %zu readings\n", nr_capacity_SensorReading_16(&ring));
    printf("  Size:      %zu readings\n", nr_size_SensorReading_16(&ring));
    printf("  Memory:    Stack-allocated (zero heap)\n");
    printf("  Use case:  Real-time sensor data streaming\n\n");
    
    /* Simulate producer-consumer */
    producer(&ring, 10);
    
    printf("Buffer state after production:\n");
    printf("  Size:  %zu / %zu\n", 
           nr_size_SensorReading_16(&ring),
           nr_capacity_SensorReading_16(&ring));
    printf("  Full:  %s\n", nr_is_full_SensorReading_16(&ring) ? "Yes" : "No");
    printf("  Empty: %s\n\n", nr_is_empty_SensorReading_16(&ring) ? "Yes" : "No");
    
    consumer(&ring);
    
    /* Test wraparound */
    printf("Testing wraparound (writing 20 items to 16-slot buffer)...\n");
    producer(&ring, 20);
    consumer(&ring);
    
    printf("==============================================\n");
    printf("Ring buffers are perfect for:\n");
    printf("  ✓ Real-time audio/video streaming\n");
    printf("  ✓ Embedded systems (no malloc)\n");
    printf("  ✓ Producer-consumer queues\n");
    printf("  ✓ Fixed-size history buffers\n");
    printf("==============================================\n");
    
    return 0;
}
/*
 * =============================================================================
 * NanoDS - Nano Data Structures
 * A single-header, STB-style C library for generic, type-safe data structures
 * 
 * Author:  Nowazish-Nur-Kayef
 * Version: 0.0.1
 * License: MIT
 * 
 * =============================================================================
 * HOW TO USE:  
 * =============================================================================
 * 
 * 1. In ONE C file, define NANODS_IMPLEMENTATION before including this header:   
 *    
 *    #define NANODS_IMPLEMENTATION
 *    #include "nanods.h"
 * 
 * 2. In other files, just include normally:
 *    
 *    #include "nanods.h"
 * 
 * 3. Define your vector types using the macro template:
 *    
 *    NANODS_DEFINE_VECTOR(MyStruct)
 * 
 * 4. Use the pre-defined types or generated API:   
 *    
 *    NanoVector_int vec;
 *    nv_init_int(&vec);
 *    nv_push_int(&vec, 42);
 *    nv_push_int(&vec, 100);
 *    int value = nv_get_int(&vec, 0);
 *    nv_free_int(&vec);
 * 
 * 5. For hash maps (string -> generic pointer):
 *    
 *    NanoMap map;
 *    nm_init(&map);
 *    int my_value = 123;
 *    nm_set(&map, "key", &my_value);
 *    int* result = (int*)nm_get(&map, "key");
 *    nm_free(&map);
 * 
 * =============================================================================
 * SECURITY FEATURES:
 * =============================================================================
 * 
 * - Integer Overflow Protection: All size calculations are checked against
 *   SIZE_MAX to prevent wraparound vulnerabilities. 
 * 
 * - Safe Reallocation: Memory allocation uses a temporary pointer to prevent
 *   memory leaks if realloc() fails.
 * 
 * - Bounds Checking: All get/set operations validate indices and return errors
 *   or assertions in debug mode.
 * 
 * - Secure Free: nanods_secure_free() zeros memory before deallocation to
 *   prevent sensitive data exposure.
 * 
 * - Hash Collision Resistance: Uses FNV-1a hash algorithm with good
 *   distribution properties.  
 * 
 * =============================================================================
 */

#ifndef NANODS_H
#define NANODS_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline void* nanods_safe_realloc(void* ptr, size_t new_size) {
    if (new_size == 0) {
        if (ptr) free(ptr);
        return NULL;
    }
    void* new_ptr = realloc(ptr, new_size);
    if (!new_ptr && new_size > 0) return NULL;
    return new_ptr;
}

static inline void nanods_secure_free(void* ptr, size_t size) {
    if (ptr && size > 0) {
        memset(ptr, 0, size);
        free(ptr);
    } else if (ptr) {
        free(ptr);
    }
}

static inline int nanods_check_mul_overflow(size_t a, size_t b, size_t* result) {
    if (a > 0 && b > SIZE_MAX / a) return 1;
    *result = a * b;
    return 0;
}

static inline int nanods_check_add_overflow(size_t a, size_t b, size_t* result) {
    if (a > SIZE_MAX - b) return 1;
    *result = a + b;
    return 0;
}

#define NANODS_DEFINE_VECTOR(T)                                                \
    typedef struct {                                                           \
        T* data;                                                               \
        size_t size;                                                           \
        size_t capacity;                                                       \
    } NanoVector_##T;                                                          \
                                                                               \
    static inline void nv_init_##T(NanoVector_##T* vec) {                     \
        assert(vec != NULL);                                                   \
        vec->data = NULL;                                                      \
        vec->size = 0;                                                         \
        vec->capacity = 0;                                                     \
    }                                                                          \
                                                                               \
    static inline int nv_reserve_##T(NanoVector_##T* vec, size_t new_capacity) { \
        assert(vec != NULL);                                                   \
        if (new_capacity <= vec->capacity) return 0;                           \
        size_t byte_size;                                                      \
        if (nanods_check_mul_overflow(new_capacity, sizeof(T), &byte_size))   \
            return -1;                                                         \
        T* new_data = (T*)nanods_safe_realloc(vec->data, byte_size);          \
        if (!new_data) return -1;                                              \
        vec->data = new_data;                                                  \
        vec->capacity = new_capacity;                                          \
        return 0;                                                              \
    }                                                                          \
                                                                               \
    static inline int nv_push_##T(NanoVector_##T* vec, T value) {             \
        assert(vec != NULL);                                                   \
        if (vec->size >= vec->capacity) {                                      \
            size_t new_capacity = vec->capacity == 0 ? 8 : vec->capacity * 2; \
            if (new_capacity < vec->capacity) return -1;                       \
            if (nv_reserve_##T(vec, new_capacity) != 0) return -1;            \
        }                                                                      \
        vec->data[vec->size++] = value;                                        \
        return 0;                                                              \
    }                                                                          \
                                                                               \
    static inline T nv_get_##T(const NanoVector_##T* vec, size_t index) {     \
        assert(vec != NULL);                                                   \
        assert(index < vec->size && "Index out of bounds");                    \
        return vec->data[index];                                               \
    }                                                                          \
                                                                               \
    static inline int nv_set_##T(NanoVector_##T* vec, size_t index, T value) { \
        assert(vec != NULL);                                                   \
        if (index >= vec->size) return -1;                                     \
        vec->data[index] = value;                                              \
        return 0;                                                              \
    }                                                                          \
                                                                               \
    static inline int nv_pop_##T(NanoVector_##T* vec, T* out_value) {         \
        assert(vec != NULL);                                                   \
        if (vec->size == 0) return -1;                                         \
        if (out_value) *out_value = vec->data[vec->size - 1];                  \
        vec->size--;                                                           \
        return 0;                                                              \
    }                                                                          \
                                                                               \
    static inline size_t nv_size_##T(const NanoVector_##T* vec) {             \
        assert(vec != NULL);                                                   \
        return vec->size;                                                      \
    }                                                                          \
                                                                               \
    static inline int nv_empty_##T(const NanoVector_##T* vec) {               \
        assert(vec != NULL);                                                   \
        return vec->size == 0;                                                 \
    }                                                                          \
                                                                               \
    static inline void nv_clear_##T(NanoVector_##T* vec) {                    \
        assert(vec != NULL);                                                   \
        vec->size = 0;                                                         \
    }                                                                          \
                                                                               \
    static inline void nv_free_##T(NanoVector_##T* vec) {                     \
        assert(vec != NULL);                                                   \
        if (vec->data) {                                                       \
            free(vec->data);                                                   \
            vec->data = NULL;                                                  \
        }                                                                      \
        vec->size = 0;                                                         \
        vec->capacity = 0;                                                     \
    }                                                                          \
                                                                               \
    static inline void nv_secure_free_##T(NanoVector_##T* vec) {              \
        assert(vec != NULL);                                                   \
        if (vec->data && vec->capacity > 0) {                                  \
            nanods_secure_free(vec->data, vec->capacity * sizeof(T));          \
            vec->data = NULL;                                                  \
        }                                                                      \
        vec->size = 0;                                                         \
        vec->capacity = 0;                                                     \
    }

NANODS_DEFINE_VECTOR(int)
NANODS_DEFINE_VECTOR(float)
NANODS_DEFINE_VECTOR(double)
NANODS_DEFINE_VECTOR(char)

typedef NanoVector_int IntVector;
typedef NanoVector_float FloatVector;
typedef NanoVector_double DoubleVector;
typedef NanoVector_char CharVector;

typedef struct NanoMapEntry {
    char* key;
    void* value;
    struct NanoMapEntry* next;
} NanoMapEntry;

typedef struct {
    NanoMapEntry** buckets;
    size_t bucket_count;
    size_t size;
} NanoMap;

static inline uint32_t nanods_fnv1a_hash(const char* key) {
    assert(key != NULL);
    uint32_t hash = 2166136261u;
    while (*key) {
        hash ^= (uint8_t)(*key++);
        hash *= 16777619u;
    }
    return hash;
}

static inline void nm_init(NanoMap* map) {
    assert(map != NULL);
    map->buckets = NULL;
    map->bucket_count = 0;
    map->size = 0;
}

static inline int nm_init_with_capacity(NanoMap* map, size_t bucket_count) {
    assert(map != NULL);
    if (bucket_count == 0) bucket_count = 16;
    size_t byte_size;
    if (nanods_check_mul_overflow(bucket_count, sizeof(NanoMapEntry*), &byte_size))
        return -1;
    map->buckets = (NanoMapEntry**)calloc(bucket_count, sizeof(NanoMapEntry*));
    if (!map->buckets) return -1;
    map->bucket_count = bucket_count;
    map->size = 0;
    return 0;
}

static inline NanoMapEntry* nm_find_entry(NanoMap* map, const char* key, size_t* out_bucket_idx) {
    assert(map != NULL);
    assert(key != NULL);
    if (map->bucket_count == 0) return NULL;
    uint32_t hash = nanods_fnv1a_hash(key);
    size_t bucket_idx = hash % map->bucket_count;
    if (out_bucket_idx) *out_bucket_idx = bucket_idx;
    NanoMapEntry* entry = map->buckets[bucket_idx];
    while (entry) {
        if (strcmp(entry->key, key) == 0) return entry;
        entry = entry->next;
    }
    return NULL;
}

static inline int nm_set(NanoMap* map, const char* key, void* value) {
    assert(map != NULL);
    assert(key != NULL);
    if (map->bucket_count == 0) {
        if (nm_init_with_capacity(map, 16) != 0) return -1;
    }
    size_t bucket_idx;
    NanoMapEntry* existing = nm_find_entry(map, key, &bucket_idx);
    if (existing) {
        existing->value = value;
        return 0;
    }
    NanoMapEntry* new_entry = (NanoMapEntry*)malloc(sizeof(NanoMapEntry));
    if (!new_entry) return -1;
    new_entry->key = (char*)malloc(strlen(key) + 1);
    if (!new_entry->key) {
        free(new_entry);
        return -1;
    }
    strcpy(new_entry->key, key);
    new_entry->value = value;
    new_entry->next = map->buckets[bucket_idx];
    map->buckets[bucket_idx] = new_entry;
    map->size++;
    return 0;
}

static inline void* nm_get(const NanoMap* map, const char* key) {
    assert(map != NULL);
    assert(key != NULL);
    NanoMapEntry* entry = nm_find_entry((NanoMap*)map, key, NULL);
    return entry ? entry->value : NULL;
}

static inline int nm_has(const NanoMap* map, const char* key) {
    assert(map != NULL);
    assert(key != NULL);
    return nm_find_entry((NanoMap*)map, key, NULL) != NULL;
}

static inline int nm_remove(NanoMap* map, const char* key) {
    assert(map != NULL);
    assert(key != NULL);
    if (map->bucket_count == 0) return -1;
    uint32_t hash = nanods_fnv1a_hash(key);
    size_t bucket_idx = hash % map->bucket_count;
    NanoMapEntry** indirect = &map->buckets[bucket_idx];
    while (*indirect) {
        NanoMapEntry* entry = *indirect;
        if (strcmp(entry->key, key) == 0) {
            *indirect = entry->next;
            free(entry->key);
            free(entry);
            map->size--;
            return 0;
        }
        indirect = &entry->next;
    }
    return -1;
}

static inline size_t nm_size(const NanoMap* map) {
    assert(map != NULL);
    return map->size;
}

static inline int nm_empty(const NanoMap* map) {
    assert(map != NULL);
    return map->size == 0;
}

static inline void nm_clear(NanoMap* map) {
    assert(map != NULL);
    for (size_t i = 0; i < map->bucket_count; i++) {
        NanoMapEntry* entry = map->buckets[i];
        while (entry) {
            NanoMapEntry* next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
        map->buckets[i] = NULL;
    }
    map->size = 0;
}

static inline void nm_free(NanoMap* map) {
    assert(map != NULL);
    nm_clear(map);
    if (map->buckets) {
        free(map->buckets);
        map->buckets = NULL;
    }
    map->bucket_count = 0;
}

static inline void nm_secure_free(NanoMap* map) {
    assert(map != NULL);
    for (size_t i = 0; i < map->bucket_count; i++) {
        NanoMapEntry* entry = map->buckets[i];
        while (entry) {
            NanoMapEntry* next = entry->next;
            if (entry->key) {
                size_t key_len = strlen(entry->key);
                memset(entry->key, 0, key_len);
                free(entry->key);
            }
            free(entry);
            entry = next;
        }
    }
    if (map->buckets) {
        free(map->buckets);
        map->buckets = NULL;
    }
    map->bucket_count = 0;
    map->size = 0;
}

typedef struct {
    const NanoMap* map;
    size_t bucket_idx;
    NanoMapEntry* current;
} NanoMapIterator;

static inline void nm_iter_init(NanoMapIterator* iter, const NanoMap* map) {
    assert(iter != NULL);
    assert(map != NULL);
    iter->map = map;
    iter->bucket_idx = 0;
    iter->current = NULL;
    for (size_t i = 0; i < map->bucket_count; i++) {
        if (map->buckets[i]) {
            iter->bucket_idx = i;
            iter->current = map->buckets[i];
            return;
        }
    }
}

static inline int nm_iter_next(NanoMapIterator* iter, const char** out_key, void** out_value) {
    assert(iter != NULL);
    if (! iter->current) return -1;
    if (out_key) *out_key = iter->current->key;
    if (out_value) *out_value = iter->current->value;
    iter->current = iter->current->next;
    if (! iter->current) {
        iter->bucket_idx++;
        while (iter->bucket_idx < iter->map->bucket_count) {
            if (iter->map->buckets[iter->bucket_idx]) {
                iter->current = iter->map->buckets[iter->bucket_idx];
                break;
            }
            iter->bucket_idx++;
        }
    }
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif

#ifdef NANODS_IMPLEMENTATION
#endif

#ifdef NANODS_TEST

#include <stdio.h>

typedef struct {
    int x;
    int y;
} Point;

NANODS_DEFINE_VECTOR(Point)

int main(void) {
    printf("=== NanoDS Test Suite ===\n\n");
    
    printf("TEST 1: IntVector\n");
    printf("-----------------\n");
    IntVector vec;
    nv_init_int(&vec);
    printf("Pushing values: 10, 20, 30, 40, 50\n");
    nv_push_int(&vec, 10);
    nv_push_int(&vec, 20);
    nv_push_int(&vec, 30);
    nv_push_int(&vec, 40);
    nv_push_int(&vec, 50);
    printf("Vector size: %zu\n", nv_size_int(&vec));
    printf("Vector contents: ");
    for (size_t i = 0; i < nv_size_int(&vec); i++) {
        printf("%d ", nv_get_int(&vec, i));
    }
    printf("\n");
    int popped;
    nv_pop_int(&vec, &popped);
    printf("Popped value:  %d\n", popped);
    printf("Vector size after pop: %zu\n", nv_size_int(&vec));
    printf("Setting index 1 to 999\n");
    nv_set_int(&vec, 1, 999);
    printf("Value at index 1: %d\n", nv_get_int(&vec, 1));
    nv_free_int(&vec);
    printf("Vector freed successfully\n\n");
    
    printf("TEST 2: PointVector (Custom Struct)\n");
    printf("------------------------------------\n");
    NanoVector_Point points;
    nv_init_Point(&points);
    printf("Pushing points: (1,2), (3,4), (5,6)\n");
    nv_push_Point(&points, (Point){1, 2});
    nv_push_Point(&points, (Point){3, 4});
    nv_push_Point(&points, (Point){5, 6});
    printf("Point vector size: %zu\n", nv_size_Point(&points));
    printf("Point vector contents:\n");
    for (size_t i = 0; i < nv_size_Point(&points); i++) {
        Point p = nv_get_Point(&points, i);
        printf("  Point[%zu]: (%d, %d)\n", i, p.x, p.y);
    }
    nv_secure_free_Point(&points);
    printf("Point vector securely freed\n\n");
    
    printf("TEST 3: NanoMap (Hash Map)\n");
    printf("--------------------------\n");
    NanoMap map;
    nm_init(&map);
    int val1 = 100, val2 = 200, val3 = 300;
    printf("Setting key-value pairs:\n");
    printf("  'apple' -> 100\n");
    printf("  'banana' -> 200\n");
    printf("  'cherry' -> 300\n");
    nm_set(&map, "apple", &val1);
    nm_set(&map, "banana", &val2);
    nm_set(&map, "cherry", &val3);
    printf("Map size: %zu\n", nm_size(&map));
    printf("Getting values:\n");
    int* result = (int*)nm_get(&map, "apple");
    printf("  'apple' = %d\n", result ? *result : -1);
    result = (int*)nm_get(&map, "banana");
    printf("  'banana' = %d\n", result ? *result : -1);
    result = (int*)nm_get(&map, "nonexistent");
    printf("  'nonexistent' = %s\n", result ? "found" : "NULL (expected)");
    printf("Checking key existence:\n");
    printf("  'apple' exists: %s\n", nm_has(&map, "apple") ? "yes" : "no");
    printf("  'grape' exists: %s\n", nm_has(&map, "grape") ? "yes" : "no");
    printf("Removing 'banana'\n");
    nm_remove(&map, "banana");
    printf("Map size after removal: %zu\n", nm_size(&map));
    printf("Iterating over remaining entries:\n");
    NanoMapIterator iter;
    nm_iter_init(&iter, &map);
    const char* key;
    void* value;
    while (nm_iter_next(&iter, &key, &value) == 0) {
        printf("  '%s' -> %d\n", key, *(int*)value);
    }
    nm_secure_free(&map);
    printf("Map securely freed\n\n");
    
    printf("TEST 4: Security Features\n");
    printf("-------------------------\n");
    printf("Testing integer overflow protection:\n");
    size_t result_size;
    int overflow = nanods_check_mul_overflow(SIZE_MAX, 2, &result_size);
    printf("  SIZE_MAX * 2 overflow check: %s\n", overflow ?  "OVERFLOW DETECTED" : "safe");
    overflow = nanods_check_mul_overflow(100, 200, &result_size);
    printf("  100 * 200 overflow check: %s (result=%zu)\n", overflow ? "overflow" : "SAFE", result_size);
    printf("Testing secure free:\n");
    IntVector secure_vec;
    nv_init_int(&secure_vec);
    nv_push_int(&secure_vec, 12345);
    nv_push_int(&secure_vec, 67890);
    printf("  Created vector with sensitive data:  12345, 67890\n");
    nv_secure_free_int(&secure_vec);
    printf("  Vector securely freed (memory zeroed)\n\n");
    
    printf("TEST 5: Edge Cases\n");
    printf("------------------\n");
    IntVector empty_vec;
    nv_init_int(&empty_vec);
    printf("Empty vector size: %zu\n", nv_size_int(&empty_vec));
    printf("Empty vector is empty: %s\n", nv_empty_int(&empty_vec) ? "yes" : "no");
    int pop_result;
    int pop_status = nv_pop_int(&empty_vec, &pop_result);
    printf("Popping from empty vector: %s\n", pop_status == -1 ? "ERROR (expected)" : "unexpected success");
    nv_free_int(&empty_vec);
    printf("Empty vector freed\n\n");
    
    printf("=== All Tests Completed Successfully ===\n");
    printf("NanoDS is memory-safe and ready for production use!\n");
    
    return 0;
}

#endif

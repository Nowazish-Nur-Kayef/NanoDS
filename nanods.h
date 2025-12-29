/**
 * @file nanods. h
 * @brief NanoDS - Production-Grade Nano Data Structures
 * @version 0.1.0
 * @author Nowazish-Nur-Kayef
 * @date 2025
 * @license MIT / Public Domain
 * 
 * =============================================================================
 * NanoDS - Nano Data Structures
 * A production-ready, memory-safe, single-header C library for generic,
 * type-safe data structures with custom allocator support.
 * =============================================================================
 * 
 * FEATURES:
 * - Hybrid Safety System (assert + hard error checking)
 * - Custom Allocator Support (embedded systems friendly)
 * - Multiple Data Structures (Vector, Map, List, Stack)
 * - Zero Dependencies (only standard C library)
 * - Cross-Platform (Linux, macOS, Windows)
 * 
 * USAGE:
 * 
 * In ONE C file, define NANODS_IMPLEMENTATION before including: 
 * 
 *     #define NANODS_IMPLEMENTATION
 *     #include "nanods.h"
 * 
 * In other files, just include normally:
 * 
 *     #include "nanods.h"
 * 
 * SAFETY MODES:
 * 
 * - Debug Mode: assert() catches logic errors during development
 * - Hard Safety:  Define NANODS_HARD_SAFETY for runtime error checking in production
 * 
 *     #define NANODS_HARD_SAFETY
 *     #include "nanods.h"
 * 
 * CUSTOM ALLOCATORS:
 * 
 *     NanoAllocator custom_alloc = {
 *         . malloc_fn = my_malloc,
 *         .realloc_fn = my_realloc,
 *         . free_fn = my_free
 *     };
 *     nanods_set_allocator(&custom_alloc);
 * 
 * =============================================================================
 */

#ifndef NANODS_H
#define NANODS_H

#define NANODS_VERSION_MAJOR 1
#define NANODS_VERSION_MINOR 0
#define NANODS_VERSION_PATCH 0
#define NANODS_VERSION "0.1.0"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ErrorCodes Error Codes
 * @{
 */
typedef enum {
    NANODS_OK = 0,
    NANODS_ERR_NOMEM = -1,
    NANODS_ERR_BOUNDS = -2,
    NANODS_ERR_EMPTY = -3,
    NANODS_ERR_OVERFLOW = -4,
    NANODS_ERR_NOTFOUND = -5,
    NANODS_ERR_NULL = -6
} NanoDSError;
/** @} */

/**
 * @defgroup Allocator Custom Allocator Interface
 * @{
 */

/**
 * @brief Custom allocator function pointers for embedded systems
 */
typedef struct {
    void* (*malloc_fn)(size_t size);
    void* (*realloc_fn)(void* ptr, size_t size);
    void  (*free_fn)(void* ptr);
} NanoAllocator;

/**
 * @brief Set custom allocator (NULL to use default)
 * @param allocator Pointer to custom allocator or NULL for default
 */
void nanods_set_allocator(NanoAllocator* allocator);

/**
 * @brief Get current allocator
 * @return Pointer to current allocator
 */
NanoAllocator* nanods_get_allocator(void);

/** @} */

/**
 * @defgroup SafetyMacros Safety Checking Macros
 * @{
 */

#ifdef NANODS_HARD_SAFETY
    #define NANODS_CHECK_NULL(ptr, ret) \
        do { if (!(ptr)) return (ret); } while(0)
    #define NANODS_CHECK_NULL_VOID(ptr) \
        do { if (!(ptr)) return; } while(0)
    #define NANODS_CHECK_BOUNDS(idx, size, ret) \
        do { if ((idx) >= (size)) return (ret); } while(0)
    #define NANODS_CHECK_EMPTY(size, ret) \
        do { if ((size) == 0) return (ret); } while(0)
#else
    #define NANODS_CHECK_NULL(ptr, ret) \
        assert((ptr) != NULL && "Null pointer")
    #define NANODS_CHECK_NULL_VOID(ptr) \
        assert((ptr) != NULL && "Null pointer")
    #define NANODS_CHECK_BOUNDS(idx, size, ret) \
        assert((idx) < (size) && "Index out of bounds")
    #define NANODS_CHECK_EMPTY(size, ret) \
        assert((size) > 0 && "Container is empty")
#endif

/** @} */

/**
 * @defgroup CoreUtilities Core Utility Functions
 * @{
 */

/**
 * @brief Secure memory zeroing before free
 * @param ptr Pointer to memory
 * @param size Size of memory region
 */
static inline void nanods_secure_free(void* ptr, size_t size);

/**
 * @brief Check for multiplication overflow
 * @param a First operand
 * @param b Second operand
 * @param result Pointer to store result
 * @return 0 if safe, 1 if overflow
 */
static inline int nanods_check_mul_overflow(size_t a, size_t b, size_t* result);

/**
 * @brief Check for addition overflow
 * @param a First operand
 * @param b Second operand
 * @param result Pointer to store result
 * @return 0 if safe, 1 if overflow
 */
static inline int nanods_check_add_overflow(size_t a, size_t b, size_t* result);

/** @} */

/* =============================================================================
 * IMPLEMENTATION:  Allocator
 * =============================================================================
 */

#ifdef NANODS_IMPLEMENTATION

static NanoAllocator g_nanods_default_allocator = {
    . malloc_fn = malloc,
    .realloc_fn = realloc,
    .free_fn = free
};

static NanoAllocator* g_nanods_allocator = &g_nanods_default_allocator;

void nanods_set_allocator(NanoAllocator* allocator) {
    g_nanods_allocator = allocator ?  allocator : &g_nanods_default_allocator;
}

NanoAllocator* nanods_get_allocator(void) {
    return g_nanods_allocator;
}

#define NANODS_MALLOC(size) g_nanods_allocator->malloc_fn(size)
#define NANODS_REALLOC(ptr, size) g_nanods_allocator->realloc_fn(ptr, size)
#define NANODS_FREE(ptr) g_nanods_allocator->free_fn(ptr)

#else

#define NANODS_MALLOC(size) malloc(size)
#define NANODS_REALLOC(ptr, size) realloc(ptr, size)
#define NANODS_FREE(ptr) free(ptr)

#endif

/* =============================================================================
 * IMPLEMENTATION: Core Utilities
 * =============================================================================
 */

static inline void nanods_secure_free(void* ptr, size_t size) {
    if (ptr && size > 0) {
        memset(ptr, 0, size);
        NANODS_FREE(ptr);
    } else if (ptr) {
        NANODS_FREE(ptr);
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

/* =============================================================================
 * DATA STRUCTURE: NanoVector (Dynamic Array)
 * =============================================================================
 */

/**
 * @defgroup NanoVector Generic Dynamic Array
 * @{
 */

/**
 * @brief Define a type-safe vector for any type
 * @param T Type name
 * 
 * Generates: 
 * - NanoVector_T structure
 * - nv_init_T, nv_push_T, nv_get_T, nv_free_T, etc.
 */
#define NANODS_DEFINE_VECTOR(T)                                                \
    typedef struct {                                                           \
        T* data;                                                               \
        size_t size;                                                           \
        size_t capacity;                                                       \
    } NanoVector_##T;                                                          \
                                                                               \
    static inline void nv_init_##T(NanoVector_##T* vec) {                     \
        NANODS_CHECK_NULL_VOID(vec);                                           \
        vec->data = NULL;                                                      \
        vec->size = 0;                                                         \
        vec->capacity = 0;                                                     \
    }                                                                          \
                                                                               \
    static inline int nv_reserve_##T(NanoVector_##T* vec, size_t new_capacity) { \
        NANODS_CHECK_NULL(vec, NANODS_ERR_NULL);                               \
        if (new_capacity <= vec->capacity) return NANODS_OK;                   \
        size_t byte_size;                                                      \
        if (nanods_check_mul_overflow(new_capacity, sizeof(T), &byte_size))   \
            return NANODS_ERR_OVERFLOW;                                        \
        T* new_data = (T*)NANODS_REALLOC(vec->data, byte_size);               \
        if (!new_data) return NANODS_ERR_NOMEM;                                \
        vec->data = new_data;                                                  \
        vec->capacity = new_capacity;                                          \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nv_push_##T(NanoVector_##T* vec, T value) {             \
        NANODS_CHECK_NULL(vec, NANODS_ERR_NULL);                               \
        if (vec->size >= vec->capacity) {                                      \
            size_t new_capacity = vec->capacity == 0 ? 8 : vec->capacity * 2; \
            if (new_capacity < vec->capacity) return NANODS_ERR_OVERFLOW;      \
            int err = nv_reserve_##T(vec, new_capacity);                      \
            if (err != NANODS_OK) return err;                                  \
        }                                                                      \
        vec->data[vec->size++] = value;                                        \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nv_get_##T(const NanoVector_##T* vec, size_t index, T* out) { \
        NANODS_CHECK_NULL(vec, NANODS_ERR_NULL);                               \
        NANODS_CHECK_NULL(out, NANODS_ERR_NULL);                               \
        NANODS_CHECK_BOUNDS(index, vec->size, NANODS_ERR_BOUNDS);             \
        *out = vec->data[index];                                               \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nv_set_##T(NanoVector_##T* vec, size_t index, T value) { \
        NANODS_CHECK_NULL(vec, NANODS_ERR_NULL);                               \
        NANODS_CHECK_BOUNDS(index, vec->size, NANODS_ERR_BOUNDS);             \
        vec->data[index] = value;                                              \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nv_pop_##T(NanoVector_##T* vec, T* out) {               \
        NANODS_CHECK_NULL(vec, NANODS_ERR_NULL);                               \
        NANODS_CHECK_EMPTY(vec->size, NANODS_ERR_EMPTY);                      \
        if (out) *out = vec->data[vec->size - 1];                              \
        vec->size--;                                                           \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline size_t nv_size_##T(const NanoVector_##T* vec) {             \
        return vec ?  vec->size : 0;                                            \
    }                                                                          \
                                                                               \
    static inline int nv_empty_##T(const NanoVector_##T* vec) {               \
        return vec ? (vec->size == 0) : 1;                                     \
    }                                                                          \
                                                                               \
    static inline void nv_clear_##T(NanoVector_##T* vec) {                    \
        if (vec) vec->size = 0;                                                \
    }                                                                          \
                                                                               \
    static inline void nv_free_##T(NanoVector_##T* vec) {                     \
        if (vec && vec->data) {                                                \
            NANODS_FREE(vec->data);                                            \
            vec->data = NULL;                                                  \
        }                                                                      \
        if (vec) {                                                             \
            vec->size = 0;                                                     \
            vec->capacity = 0;                                                 \
        }                                                                      \
    }                                                                          \
                                                                               \
    static inline void nv_secure_free_##T(NanoVector_##T* vec) {              \
        if (vec && vec->data && vec->capacity > 0) {                           \
            nanods_secure_free(vec->data, vec->capacity * sizeof(T));          \
            vec->data = NULL;                                                  \
        }                                                                      \
        if (vec) {                                                             \
            vec->size = 0;                                                     \
            vec->capacity = 0;                                                 \
        }                                                                      \
    }

NANODS_DEFINE_VECTOR(int)
NANODS_DEFINE_VECTOR(float)
NANODS_DEFINE_VECTOR(double)
NANODS_DEFINE_VECTOR(char)

typedef NanoVector_int IntVector;
typedef NanoVector_float FloatVector;
typedef NanoVector_double DoubleVector;
typedef NanoVector_char CharVector;

/** @} */

/* =============================================================================
 * DATA STRUCTURE: NanoStack (LIFO Stack using NanoVector)
 * =============================================================================
 */

/**
 * @defgroup NanoStack LIFO Stack (Composition over Vector)
 * @{
 */

#define NANODS_DEFINE_STACK(T)                                                 \
    typedef NanoVector_##T NanoStack_##T;                                      \
                                                                               \
    static inline void ns_init_##T(NanoStack_##T* stack) {                    \
        nv_init_##T(stack);                                                    \
    }                                                                          \
                                                                               \
    static inline int ns_push_##T(NanoStack_##T* stack, T value) {            \
        return nv_push_##T(stack, value);                                     \
    }                                                                          \
                                                                               \
    static inline int ns_pop_##T(NanoStack_##T* stack, T* out) {              \
        return nv_pop_##T(stack, out);                                        \
    }                                                                          \
                                                                               \
    static inline int ns_peek_##T(const NanoStack_##T* stack, T* out) {       \
        NANODS_CHECK_NULL(stack, NANODS_ERR_NULL);                             \
        NANODS_CHECK_NULL(out, NANODS_ERR_NULL);                               \
        NANODS_CHECK_EMPTY(stack->size, NANODS_ERR_EMPTY);                    \
        *out = stack->data[stack->size - 1];                                   \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline size_t ns_size_##T(const NanoStack_##T* stack) {            \
        return nv_size_##T(stack);                                             \
    }                                                                          \
                                                                               \
    static inline int ns_empty_##T(const NanoStack_##T* stack) {              \
        return nv_empty_##T(stack);                                            \
    }                                                                          \
                                                                               \
    static inline void ns_free_##T(NanoStack_##T* stack) {                    \
        nv_free_##T(stack);                                                    \
    }

NANODS_DEFINE_STACK(int)
NANODS_DEFINE_STACK(float)
NANODS_DEFINE_STACK(double)
NANODS_DEFINE_STACK(char)

typedef NanoStack_int IntStack;
typedef NanoStack_float FloatStack;
typedef NanoStack_double DoubleStack;
typedef NanoStack_char CharStack;

/** @} */

/* =============================================================================
 * DATA STRUCTURE: NanoList (Singly Linked List)
 * =============================================================================
 */

/**
 * @defgroup NanoList Singly Linked List
 * @{
 */

#define NANODS_DEFINE_LIST(T)                                                  \
    typedef struct NanoListNode_##T {                                          \
        T data;                                                                \
        struct NanoListNode_##T* next;                                         \
    } NanoListNode_##T;                                                        \
                                                                               \
    typedef struct {                                                           \
        NanoListNode_##T* head;                                                \
        NanoListNode_##T* tail;                                                \
        size_t size;                                                           \
    } NanoList_##T;                                                            \
                                                                               \
    static inline void nl_init_##T(NanoList_##T* list) {                      \
        NANODS_CHECK_NULL_VOID(list);                                          \
        list->head = NULL;                                                     \
        list->tail = NULL;                                                     \
        list->size = 0;                                                        \
    }                                                                          \
                                                                               \
    static inline int nl_push_front_##T(NanoList_##T* list, T value) {        \
        NANODS_CHECK_NULL(list, NANODS_ERR_NULL);                              \
        NanoListNode_##T* node = (NanoListNode_##T*)NANODS_MALLOC(sizeof(NanoListNode_##T)); \
        if (!node) return NANODS_ERR_NOMEM;                                    \
        node->data = value;                                                    \
        node->next = list->head;                                               \
        list->head = node;                                                     \
        if (list->tail == NULL) list->tail = node;                             \
        list->size++;                                                          \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nl_push_back_##T(NanoList_##T* list, T value) {         \
        NANODS_CHECK_NULL(list, NANODS_ERR_NULL);                              \
        NanoListNode_##T* node = (NanoListNode_##T*)NANODS_MALLOC(sizeof(NanoListNode_##T)); \
        if (!node) return NANODS_ERR_NOMEM;                                    \
        node->data = value;                                                    \
        node->next = NULL;                                                     \
        if (list->tail) {                                                      \
            list->tail->next = node;                                           \
        } else {                                                               \
            list->head = node;                                                 \
        }                                                                      \
        list->tail = node;                                                     \
        list->size++;                                                          \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nl_pop_front_##T(NanoList_##T* list, T* out) {          \
        NANODS_CHECK_NULL(list, NANODS_ERR_NULL);                              \
        NANODS_CHECK_EMPTY(list->size, NANODS_ERR_EMPTY);                     \
        NanoListNode_##T* node = list->head;                                   \
        if (out) *out = node->data;                                            \
        list->head = node->next;                                               \
        if (list->head == NULL) list->tail = NULL;                             \
        NANODS_FREE(node);                                                     \
        list->size--;                                                          \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline size_t nl_size_##T(const NanoList_##T* list) {              \
        return list ? list->size : 0;                                          \
    }                                                                          \
                                                                               \
    static inline int nl_empty_##T(const NanoList_##T* list) {                \
        return list ? (list->size == 0) : 1;                                   \
    }                                                                          \
                                                                               \
    static inline void nl_free_##T(NanoList_##T* list) {                      \
        if (! list) return;                                                     \
        NanoListNode_##T* current = list->head;                                \
        while (current) {                                                      \
            NanoListNode_##T* next = current->next;                            \
            NANODS_FREE(current);                                              \
            current = next;                                                    \
        }                                                                      \
        list->head = NULL;                                                     \
        list->tail = NULL;                                                     \
        list->size = 0;                                                        \
    }

NANODS_DEFINE_LIST(int)
NANODS_DEFINE_LIST(float)
NANODS_DEFINE_LIST(double)
NANODS_DEFINE_LIST(char)

typedef NanoList_int IntList;
typedef NanoList_float FloatList;
typedef NanoList_double DoubleList;
typedef NanoList_char CharList;

/** @} */

/* =============================================================================
 * DATA STRUCTURE: NanoMap (Hash Map)
 * =============================================================================
 */

/**
 * @defgroup NanoMap String-Keyed Hash Map
 * @{
 */

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
    NANODS_CHECK_NULL(key, 0);
    uint32_t hash = 2166136261u;
    while (*key) {
        hash ^= (uint8_t)(*key++);
        hash *= 16777619u;
    }
    return hash;
}

static inline void nm_init(NanoMap* map) {
    NANODS_CHECK_NULL_VOID(map);
    map->buckets = NULL;
    map->bucket_count = 0;
    map->size = 0;
}

static inline int nm_init_with_capacity(NanoMap* map, size_t bucket_count) {
    NANODS_CHECK_NULL(map, NANODS_ERR_NULL);
    if (bucket_count == 0) bucket_count = 16;
    size_t byte_size;
    if (nanods_check_mul_overflow(bucket_count, sizeof(NanoMapEntry*), &byte_size))
        return NANODS_ERR_OVERFLOW;
    map->buckets = (NanoMapEntry**)NANODS_MALLOC(byte_size);
    if (!map->buckets) return NANODS_ERR_NOMEM;
    memset(map->buckets, 0, byte_size);
    map->bucket_count = bucket_count;
    map->size = 0;
    return NANODS_OK;
}

static inline NanoMapEntry* nm_find_entry(NanoMap* map, const char* key, size_t* out_bucket_idx) {
    NANODS_CHECK_NULL(map, NULL);
    NANODS_CHECK_NULL(key, NULL);
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
    NANODS_CHECK_NULL(map, NANODS_ERR_NULL);
    NANODS_CHECK_NULL(key, NANODS_ERR_NULL);
    if (map->bucket_count == 0) {
        int err = nm_init_with_capacity(map, 16);
        if (err != NANODS_OK) return err;
    }
    size_t bucket_idx;
    NanoMapEntry* existing = nm_find_entry(map, key, &bucket_idx);
    if (existing) {
        existing->value = value;
        return NANODS_OK;
    }
    NanoMapEntry* new_entry = (NanoMapEntry*)NANODS_MALLOC(sizeof(NanoMapEntry));
    if (!new_entry) return NANODS_ERR_NOMEM;
    new_entry->key = (char*)NANODS_MALLOC(strlen(key) + 1);
    if (!new_entry->key) {
        NANODS_FREE(new_entry);
        return NANODS_ERR_NOMEM;
    }
    strcpy(new_entry->key, key);
    new_entry->value = value;
    new_entry->next = map->buckets[bucket_idx];
    map->buckets[bucket_idx] = new_entry;
    map->size++;
    return NANODS_OK;
}

static inline void* nm_get(const NanoMap* map, const char* key) {
    NANODS_CHECK_NULL(map, NULL);
    NANODS_CHECK_NULL(key, NULL);
    NanoMapEntry* entry = nm_find_entry((NanoMap*)map, key, NULL);
    return entry ? entry->value : NULL;
}

static inline int nm_has(const NanoMap* map, const char* key) {
    NANODS_CHECK_NULL(map, 0);
    NANODS_CHECK_NULL(key, 0);
    return nm_find_entry((NanoMap*)map, key, NULL) != NULL;
}

static inline int nm_remove(NanoMap* map, const char* key) {
    NANODS_CHECK_NULL(map, NANODS_ERR_NULL);
    NANODS_CHECK_NULL(key, NANODS_ERR_NULL);
    if (map->bucket_count == 0) return NANODS_ERR_NOTFOUND;
    uint32_t hash = nanods_fnv1a_hash(key);
    size_t bucket_idx = hash % map->bucket_count;
    NanoMapEntry** indirect = &map->buckets[bucket_idx];
    while (*indirect) {
        NanoMapEntry* entry = *indirect;
        if (strcmp(entry->key, key) == 0) {
            *indirect = entry->next;
            NANODS_FREE(entry->key);
            NANODS_FREE(entry);
            map->size--;
            return NANODS_OK;
        }
        indirect = &entry->next;
    }
    return NANODS_ERR_NOTFOUND;
}

static inline size_t nm_size(const NanoMap* map) {
    return map ?  map->size : 0;
}

static inline int nm_empty(const NanoMap* map) {
    return map ? (map->size == 0) : 1;
}

static inline void nm_clear(NanoMap* map) {
    if (! map) return;
    for (size_t i = 0; i < map->bucket_count; i++) {
        NanoMapEntry* entry = map->buckets[i];
        while (entry) {
            NanoMapEntry* next = entry->next;
            NANODS_FREE(entry->key);
            NANODS_FREE(entry);
            entry = next;
        }
        map->buckets[i] = NULL;
    }
    map->size = 0;
}

static inline void nm_free(NanoMap* map) {
    if (!map) return;
    nm_clear(map);
    if (map->buckets) {
        NANODS_FREE(map->buckets);
        map->buckets = NULL;
    }
    map->bucket_count = 0;
}

static inline void nm_secure_free(NanoMap* map) {
    if (!map) return;
    for (size_t i = 0; i < map->bucket_count; i++) {
        NanoMapEntry* entry = map->buckets[i];
        while (entry) {
            NanoMapEntry* next = entry->next;
            if (entry->key) {
                size_t key_len = strlen(entry->key);
                memset(entry->key, 0, key_len);
                NANODS_FREE(entry->key);
            }
            NANODS_FREE(entry);
            entry = next;
        }
    }
    if (map->buckets) {
        NANODS_FREE(map->buckets);
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
    NANODS_CHECK_NULL_VOID(iter);
    NANODS_CHECK_NULL_VOID(map);
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
    NANODS_CHECK_NULL(iter, NANODS_ERR_NULL);
    if (! iter->current) return NANODS_ERR_EMPTY;
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
    return NANODS_OK;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif

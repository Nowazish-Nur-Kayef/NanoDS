/**
 * =============================================================================
 * NanoDS v1.0.0 - BUNDLED SINGLE-FILE VERSION
 * =============================================================================
 * 
 * This is an automatically generated single-file distribution of NanoDS.
 * Generated on:  2026-02-01 10:44:45
 * 
 * For the modular source, see: https://github.com/Nowazish-Nur-Kayef/NanoDS
 * 
 * This file contains the entire NanoDS library in one header. 
 * Simply #include this file and #define NANODS_IMPLEMENTATION in ONE . c file.
 * 
 * =============================================================================
 */

/**
 * @file nanods.h
 * @brief NanoDS - Production-Grade Nano Data Structures
 * @version 1.0.0
 * @author Nowazish-Nur-Kayef
 * @date 2025
 * @license MIT / Public Domain
 * 
 * =============================================================================
 * NanoDS - Nano Data Structures
 * Production-ready, memory-safe, single-header C library for generic,
 * type-safe data structures with anti-DoS security and custom allocator support.
 * =============================================================================
 */

#ifndef NANODS_H
#define NANODS_H

#define NANODS_VERSION_MAJOR 1
#define NANODS_VERSION_MINOR 0
#define NANODS_VERSION_PATCH 0
#define NANODS_VERSION "1.0.0"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Compiler hints for optimization */
#ifdef __GNUC__
    #define NANODS_LIKELY(x)   __builtin_expect(!!(x), 1)
    #define NANODS_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define NANODS_LIKELY(x)   (x)
    #define NANODS_UNLIKELY(x) (x)
#endif

/* Flags for initialization */
#define NANODS_FLAG_NONE   0x00
#define NANODS_FLAG_SECURE 0x01  /* Automatic secure wipe on free */

/* ========== BUNDLED IMPLEMENTATION - ALL FILES COMBINED ========== */

/* ==================== src/core.h ==================== */
/**
 * @file core.h
 * @brief Core utilities, allocator interface, security, and flags
 */

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
    NANODS_ERR_NULL = -6,
    NANODS_ERR_FULL = -7        /* NEW: For ring buffer */
} NanoDSError;
/** @} */

/**
 * @defgroup Allocator Custom Allocator Interface
 * @{
 */
typedef struct {
    void* (*malloc_fn)(size_t size);
    void* (*realloc_fn)(void* ptr, size_t size);
    void  (*free_fn)(void* ptr);
} NanoAllocator;

void nanods_set_allocator(NanoAllocator* allocator);
NanoAllocator* nanods_get_allocator(void);
/** @} */

/**
 * @defgroup Security Security Functions
 * @{
 */

/**
 * Initialize global seed for hash randomization (Anti-DoS)
 * Call once at program startup, or pass custom seed
 */
void nanods_seed_init(uint32_t custom_seed);

/**
 * Get current hash seed
 */
uint32_t nanods_get_seed(void);

/** @} */

/**
 * @defgroup SafetyMacros Safety Checking Macros
 * @{
 */
#ifdef NANODS_HARD_SAFETY
    #define NANODS_CHECK_NULL(ptr, ret) \
        do { if (NANODS_UNLIKELY(!(ptr))) return (ret); } while(0)
    #define NANODS_CHECK_NULL_VOID(ptr) \
        do { if (NANODS_UNLIKELY(!(ptr))) return; } while(0)
    #define NANODS_CHECK_BOUNDS(idx, size, ret) \
        do { if (NANODS_UNLIKELY((idx) >= (size))) return (ret); } while(0)
    #define NANODS_CHECK_EMPTY(size, ret) \
        do { if (NANODS_UNLIKELY((size) == 0)) return (ret); } while(0)
    #define NANODS_CHECK_FULL(size, capacity, ret) \
        do { if (NANODS_UNLIKELY((size) >= (capacity))) return (ret); } while(0)
#else
    #define NANODS_CHECK_NULL(ptr, ret) \
        assert((ptr) != NULL && "Null pointer")
    #define NANODS_CHECK_NULL_VOID(ptr) \
        assert((ptr) != NULL && "Null pointer")
    #define NANODS_CHECK_BOUNDS(idx, size, ret) \
        assert((idx) < (size) && "Index out of bounds")
    #define NANODS_CHECK_EMPTY(size, ret) \
        assert((size) > 0 && "Container is empty")
    #define NANODS_CHECK_FULL(size, capacity, ret) \
        assert((size) < (capacity) && "Container is full")
#endif
/** @} */

/**
 * @defgroup CoreUtilities Core Utility Functions
 * @{
 */
static inline void nanods_secure_free(void* ptr, size_t size);
static inline int nanods_check_mul_overflow(size_t a, size_t b, size_t* result);
static inline int nanods_check_add_overflow(size_t a, size_t b, size_t* result);
/** @} */

/* =============================================================================
 * IMPLEMENTATION
 * =============================================================================
 */

#ifdef NANODS_IMPLEMENTATION

static NanoAllocator g_nanods_default_allocator = {
    .malloc_fn = malloc,
    .realloc_fn = realloc,
    .free_fn = free
};

static NanoAllocator* g_nanods_allocator = &g_nanods_default_allocator;
static uint32_t g_nanods_hash_seed = 0;
static int g_nanods_seed_initialized = 0;

void nanods_set_allocator(NanoAllocator* allocator) {
    g_nanods_allocator = allocator ?  allocator : &g_nanods_default_allocator;
}

NanoAllocator* nanods_get_allocator(void) {
    return g_nanods_allocator;
}

void nanods_seed_init(uint32_t custom_seed) {
    if (custom_seed == 0) {
        /* Generate seed from time + address space randomization */
        uint32_t t = (uint32_t)time(NULL);
        uintptr_t p = (uintptr_t)&g_nanods_hash_seed;
        g_nanods_hash_seed = t ^ (uint32_t)(p >> 16) ^ (uint32_t)(p & 0xFFFF);
    } else {
        g_nanods_hash_seed = custom_seed;
    }
    g_nanods_seed_initialized = 1;
}

uint32_t nanods_get_seed(void) {
    if (NANODS_UNLIKELY(!g_nanods_seed_initialized)) {
        nanods_seed_init(0);
    }
    return g_nanods_hash_seed;
}

#define NANODS_MALLOC(size) g_nanods_allocator->malloc_fn(size)
#define NANODS_REALLOC(ptr, size) g_nanods_allocator->realloc_fn(ptr, size)
#define NANODS_FREE(ptr) g_nanods_allocator->free_fn(ptr)

#else

#define NANODS_MALLOC(size) malloc(size)
#define NANODS_REALLOC(ptr, size) realloc(ptr, size)
#define NANODS_FREE(ptr) free(ptr)

#endif

static inline void nanods_secure_free(void* ptr, size_t size) {
    if (NANODS_LIKELY(ptr && size > 0)) {
        memset(ptr, 0, size);
        NANODS_FREE(ptr);
    } else if (ptr) {
        NANODS_FREE(ptr);
    }
}

static inline int nanods_check_mul_overflow(size_t a, size_t b, size_t* result) {
    if (NANODS_UNLIKELY(a > 0 && b > SIZE_MAX / a)) return 1;
    *result = a * b;
    return 0;
}

static inline int nanods_check_add_overflow(size_t a, size_t b, size_t* result) {
    if (NANODS_UNLIKELY(a > SIZE_MAX - b)) return 1;
    *result = a + b;
    return 0;
}

#endif /* NANODS_CORE_H */
/* ==================== src/vector_impl.h ==================== */
/**
 * @file vector_impl.h
 * @brief Generic dynamic array implementation with functional operations
 */

/**
 * @defgroup NanoVector Generic Dynamic Array
 * @{
 */

#define NANODS_DEFINE_VECTOR(T)                                                \
    typedef struct {                                                           \
        T* data;                                                               \
        size_t size;                                                           \
        size_t capacity;                                                       \
        uint8_t flags;                                                         \
    } NanoVector_##T;                                                          \
                                                                               \
    static inline void nv_init_##T(NanoVector_##T* vec) {                     \
        NANODS_CHECK_NULL_VOID(vec);                                           \
        vec->data = NULL;                                                      \
        vec->size = 0;                                                         \
        vec->capacity = 0;                                                     \
        vec->flags = NANODS_FLAG_NONE;                                         \
    }                                                                          \
                                                                               \
    static inline void nv_init_ex_##T(NanoVector_##T* vec, uint8_t flags) {   \
        NANODS_CHECK_NULL_VOID(vec);                                           \
        vec->data = NULL;                                                      \
        vec->size = 0;                                                         \
        vec->capacity = 0;                                                     \
        vec->flags = flags;                                                    \
    }                                                                          \
                                                                               \
    static inline int nv_reserve_##T(NanoVector_##T* vec, size_t new_capacity) { \
        NANODS_CHECK_NULL(vec, NANODS_ERR_NULL);                               \
        if (new_capacity <= vec->capacity) return NANODS_OK;                   \
        size_t byte_size;                                                      \
        if (NANODS_UNLIKELY(nanods_check_mul_overflow(new_capacity, sizeof(T), &byte_size))) \
            return NANODS_ERR_OVERFLOW;                                        \
        T* new_data = (T*)NANODS_REALLOC(vec->data, byte_size);               \
        if (NANODS_UNLIKELY(! new_data)) return NANODS_ERR_NOMEM;              \
        vec->data = new_data;                                                  \
        vec->capacity = new_capacity;                                          \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nv_push_##T(NanoVector_##T* vec, T value) {             \
        NANODS_CHECK_NULL(vec, NANODS_ERR_NULL);                               \
        if (NANODS_UNLIKELY(vec->size >= vec->capacity)) {                     \
            size_t new_capacity = vec->capacity == 0 ? 8 : vec->capacity * 2; \
            if (NANODS_UNLIKELY(new_capacity < vec->capacity)) return NANODS_ERR_OVERFLOW; \
            int err = nv_reserve_##T(vec, new_capacity);                      \
            if (NANODS_UNLIKELY(err != NANODS_OK)) return err;                 \
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
            if (vec->flags & NANODS_FLAG_SECURE) {                             \
                nanods_secure_free(vec->data, vec->capacity * sizeof(T));      \
            } else {                                                           \
                NANODS_FREE(vec->data);                                        \
            }                                                                  \
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
    }                                                                          \
                                                                               \
    static inline int nv_map_##T(const NanoVector_##T* vec, NanoVector_##T* out, \
                                  T (*func)(T)) {                              \
        NANODS_CHECK_NULL(vec, NANODS_ERR_NULL);                               \
        NANODS_CHECK_NULL(out, NANODS_ERR_NULL);                               \
        NANODS_CHECK_NULL(func, NANODS_ERR_NULL);                              \
        nv_init_##T(out);                                                      \
        for (size_t i = 0; i < vec->size; i++) {                               \
            T mapped = func(vec->data[i]);                                     \
            int err = nv_push_##T(out, mapped);                                \
            if (NANODS_UNLIKELY(err != NANODS_OK)) {                           \
                nv_free_##T(out);                                              \
                return err;                                                    \
            }                                                                  \
        }                                                                      \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nv_filter_##T(const NanoVector_##T* vec, NanoVector_##T* out, \
                                     int (*predicate)(T)) {                    \
        NANODS_CHECK_NULL(vec, NANODS_ERR_NULL);                               \
        NANODS_CHECK_NULL(out, NANODS_ERR_NULL);                               \
        NANODS_CHECK_NULL(predicate, NANODS_ERR_NULL);                         \
        nv_init_##T(out);                                                      \
        for (size_t i = 0; i < vec->size; i++) {                               \
            if (predicate(vec->data[i])) {                                     \
                int err = nv_push_##T(out, vec->data[i]);                      \
                if (NANODS_UNLIKELY(err != NANODS_OK)) {                       \
                    nv_free_##T(out);                                          \
                    return err;                                                \
                }                                                              \
            }                                                                  \
        }                                                                      \
        return NANODS_OK;                                                      \
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

#endif /* NANODS_VECTOR_IMPL_H */
/* ==================== src/stack_impl.h ==================== */
/**
 * @file stack_impl.h
 * @brief LIFO Stack implementation (composition over vector)
 */

#define NANODS_DEFINE_STACK(T)                                                 \
    typedef NanoVector_##T NanoStack_##T;                                      \
                                                                               \
    static inline void ns_init_##T(NanoStack_##T* stack) {                    \
        nv_init_##T(stack);                                                    \
    }                                                                          \
                                                                               \
    static inline void ns_init_ex_##T(NanoStack_##T* stack, uint8_t flags) {  \
        nv_init_ex_##T(stack, flags);                                          \
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

#endif /* NANODS_STACK_IMPL_H */
/* ==================== src/list_impl.h ==================== */
/**
 * @file list_impl.h
 * @brief Singly linked list implementation
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
        uint8_t flags;                                                         \
    } NanoList_##T;                                                            \
                                                                               \
    static inline void nl_init_##T(NanoList_##T* list) {                      \
        NANODS_CHECK_NULL_VOID(list);                                          \
        list->head = NULL;                                                     \
        list->tail = NULL;                                                     \
        list->size = 0;                                                        \
        list->flags = NANODS_FLAG_NONE;                                        \
    }                                                                          \
                                                                               \
    static inline void nl_init_ex_##T(NanoList_##T* list, uint8_t flags) {    \
        NANODS_CHECK_NULL_VOID(list);                                          \
        list->head = NULL;                                                     \
        list->tail = NULL;                                                     \
        list->size = 0;                                                        \
        list->flags = flags;                                                   \
    }                                                                          \
                                                                               \
    static inline int nl_push_front_##T(NanoList_##T* list, T value) {        \
        NANODS_CHECK_NULL(list, NANODS_ERR_NULL);                              \
        NanoListNode_##T* node = (NanoListNode_##T*)NANODS_MALLOC(sizeof(NanoListNode_##T)); \
        if (NANODS_UNLIKELY(!node)) return NANODS_ERR_NOMEM;                   \
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
        if (NANODS_UNLIKELY(!  node)) return NANODS_ERR_NOMEM;                   \
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
        if (list->flags & NANODS_FLAG_SECURE) {                                \
            nanods_secure_free(node, sizeof(NanoListNode_##T));                \
        } else {                                                               \
            NANODS_FREE(node);                                                 \
        }                                                                      \
        list->size--;                                                          \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline size_t nl_size_##T(const NanoList_##T* list) {              \
        return list ?   list->size : 0;                                          \
    }                                                                          \
                                                                               \
    static inline int nl_empty_##T(const NanoList_##T* list) {                \
        return list ?   (list->size == 0) : 1;                                   \
    }                                                                          \
                                                                               \
    static inline void nl_free_##T(NanoList_##T* list) {                      \
        if (! list) return;                                                     \
        NanoListNode_##T* current = list->head;                                \
        int secure = (list->flags & NANODS_FLAG_SECURE);                       \
        while (current) {                                                      \
            NanoListNode_##T* next = current->next;                            \
            if (secure) {                                                      \
                nanods_secure_free(current, sizeof(NanoListNode_##T));         \
            } else {                                                           \
                NANODS_FREE(current);                                          \
            }                                                                  \
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

#endif /* NANODS_LIST_IMPL_H */
/* ==================== src/list2_impl.h ==================== */
/**
 * @file list2_impl.h
 * @brief Doubly linked list implementation
 */

/**
 * @defgroup NanoList2 Doubly Linked List
 * @{
 */

#define NANODS_DEFINE_LIST2(T)                                                 \
    typedef struct NanoList2Node_##T {                                         \
        T data;                                                                \
        struct NanoList2Node_##T* prev;                                        \
        struct NanoList2Node_##T* next;                                        \
    } NanoList2Node_##T;                                                       \
                                                                               \
    typedef struct {                                                           \
        NanoList2Node_##T* head;                                               \
        NanoList2Node_##T* tail;                                               \
        size_t size;                                                           \
        uint8_t flags;                                                         \
    } NanoList2_##T;                                                           \
                                                                               \
    static inline void nl2_init_##T(NanoList2_##T* list) {                    \
        NANODS_CHECK_NULL_VOID(list);                                          \
        list->head = NULL;                                                     \
        list->tail = NULL;                                                     \
        list->size = 0;                                                        \
        list->flags = NANODS_FLAG_NONE;                                        \
    }                                                                          \
                                                                               \
    static inline void nl2_init_ex_##T(NanoList2_##T* list, uint8_t flags) {  \
        NANODS_CHECK_NULL_VOID(list);                                          \
        list->head = NULL;                                                     \
        list->tail = NULL;                                                     \
        list->size = 0;                                                        \
        list->flags = flags;                                                   \
    }                                                                          \
                                                                               \
    static inline int nl2_push_front_##T(NanoList2_##T* list, T value) {      \
        NANODS_CHECK_NULL(list, NANODS_ERR_NULL);                              \
        NanoList2Node_##T* node = (NanoList2Node_##T*)NANODS_MALLOC(sizeof(NanoList2Node_##T)); \
        if (NANODS_UNLIKELY(!node)) return NANODS_ERR_NOMEM;                   \
        node->data = value;                                                    \
        node->prev = NULL;                                                     \
        node->next = list->head;                                               \
        if (list->head) {                                                      \
            list->head->prev = node;                                           \
        } else {                                                               \
            list->tail = node;                                                 \
        }                                                                      \
        list->head = node;                                                     \
        list->size++;                                                          \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nl2_push_back_##T(NanoList2_##T* list, T value) {       \
        NANODS_CHECK_NULL(list, NANODS_ERR_NULL);                              \
        NanoList2Node_##T* node = (NanoList2Node_##T*)NANODS_MALLOC(sizeof(NanoList2Node_##T)); \
        if (NANODS_UNLIKELY(! node)) return NANODS_ERR_NOMEM;                   \
        node->data = value;                                                    \
        node->prev = list->tail;                                               \
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
    static inline int nl2_pop_front_##T(NanoList2_##T* list, T* out) {        \
        NANODS_CHECK_NULL(list, NANODS_ERR_NULL);                              \
        NANODS_CHECK_EMPTY(list->size, NANODS_ERR_EMPTY);                     \
        NanoList2Node_##T* node = list->head;                                  \
        if (out) *out = node->data;                                            \
        list->head = node->next;                                               \
        if (list->head) {                                                      \
            list->head->prev = NULL;                                           \
        } else {                                                               \
            list->tail = NULL;                                                 \
        }                                                                      \
        if (list->flags & NANODS_FLAG_SECURE) {                                \
            nanods_secure_free(node, sizeof(NanoList2Node_##T));               \
        } else {                                                               \
            NANODS_FREE(node);                                                 \
        }                                                                      \
        list->size--;                                                          \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nl2_pop_back_##T(NanoList2_##T* list, T* out) {         \
        NANODS_CHECK_NULL(list, NANODS_ERR_NULL);                              \
        NANODS_CHECK_EMPTY(list->size, NANODS_ERR_EMPTY);                     \
        NanoList2Node_##T* node = list->tail;                                  \
        if (out) *out = node->data;                                            \
        list->tail = node->prev;                                               \
        if (list->tail) {                                                      \
            list->tail->next = NULL;                                           \
        } else {                                                               \
            list->head = NULL;                                                 \
        }                                                                      \
        if (list->flags & NANODS_FLAG_SECURE) {                                \
            nanods_secure_free(node, sizeof(NanoList2Node_##T));               \
        } else {                                                               \
            NANODS_FREE(node);                                                 \
        }                                                                      \
        list->size--;                                                          \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nl2_insert_after_##T(NanoList2_##T* list,               \
                                            NanoList2Node_##T* node,           \
                                            T value) {                         \
        NANODS_CHECK_NULL(list, NANODS_ERR_NULL);                              \
        NANODS_CHECK_NULL(node, NANODS_ERR_NULL);                              \
        NanoList2Node_##T* new_node = (NanoList2Node_##T*)NANODS_MALLOC(sizeof(NanoList2Node_##T)); \
        if (NANODS_UNLIKELY(!new_node)) return NANODS_ERR_NOMEM;               \
        new_node->data = value;                                                \
        new_node->prev = node;                                                 \
        new_node->next = node->next;                                           \
        if (node->next) {                                                      \
            node->next->prev = new_node;                                       \
        } else {                                                               \
            list->tail = new_node;                                             \
        }                                                                      \
        node->next = new_node;                                                 \
        list->size++;                                                          \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nl2_remove_node_##T(NanoList2_##T* list,                \
                                           NanoList2Node_##T* node) {          \
        NANODS_CHECK_NULL(list, NANODS_ERR_NULL);                              \
        NANODS_CHECK_NULL(node, NANODS_ERR_NULL);                              \
        if (node->prev) {                                                      \
            node->prev->next = node->next;                                     \
        } else {                                                               \
            list->head = node->next;                                           \
        }                                                                      \
        if (node->next) {                                                      \
            node->next->prev = node->prev;                                     \
        } else {                                                               \
            list->tail = node->prev;                                           \
        }                                                                      \
        if (list->flags & NANODS_FLAG_SECURE) {                                \
            nanods_secure_free(node, sizeof(NanoList2Node_##T));               \
        } else {                                                               \
            NANODS_FREE(node);                                                 \
        }                                                                      \
        list->size--;                                                          \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline size_t nl2_size_##T(const NanoList2_##T* list) {            \
        return list ? list->size : 0;                                          \
    }                                                                          \
                                                                               \
    static inline int nl2_empty_##T(const NanoList2_##T* list) {              \
        return list ? (list->size == 0) : 1;                                   \
    }                                                                          \
                                                                               \
    static inline void nl2_free_##T(NanoList2_##T* list) {                    \
        if (!list) return;                                                     \
        NanoList2Node_##T* current = list->head;                               \
        int secure = (list->flags & NANODS_FLAG_SECURE);                       \
        while (current) {                                                      \
            NanoList2Node_##T* next = current->next;                           \
            if (secure) {                                                      \
                nanods_secure_free(current, sizeof(NanoList2Node_##T));        \
            } else {                                                           \
                NANODS_FREE(current);                                          \
            }                                                                  \
            current = next;                                                    \
        }                                                                      \
        list->head = NULL;                                                     \
        list->tail = NULL;                                                     \
        list->size = 0;                                                        \
    }

NANODS_DEFINE_LIST2(int)
NANODS_DEFINE_LIST2(float)
NANODS_DEFINE_LIST2(double)
NANODS_DEFINE_LIST2(char)

typedef NanoList2_int IntList2;
typedef NanoList2_float FloatList2;
typedef NanoList2_double DoubleList2;
typedef NanoList2_char CharList2;

/** @} */

#endif /* NANODS_LIST2_IMPL_H */
/* ==================== src/ring_impl.h ==================== */
/**
 * @file ring_impl.h
 * @brief Fixed-size circular buffer implementation
 */

/**
 * @defgroup NanoRing Circular Buffer (Ring Buffer)
 * @{
 */

#define NANODS_DEFINE_RING(T, SIZE)                                            \
    typedef struct {                                                           \
        T data[SIZE];                                                          \
        size_t head;                                                           \
        size_t tail;                                                           \
        size_t count;                                                          \
        size_t capacity;                                                       \
        uint8_t flags;                                                         \
    } NanoRing_##T##_##SIZE;                                                   \
                                                                               \
    static inline void nr_init_##T##_##SIZE(NanoRing_##T##_##SIZE* ring) {    \
        NANODS_CHECK_NULL_VOID(ring);                                          \
        ring->head = 0;                                                        \
        ring->tail = 0;                                                        \
        ring->count = 0;                                                       \
        ring->capacity = SIZE;                                                 \
        ring->flags = NANODS_FLAG_NONE;                                        \
    }                                                                          \
                                                                               \
    static inline void nr_init_ex_##T##_##SIZE(NanoRing_##T##_##SIZE* ring,   \
                                                 uint8_t flags) {              \
        NANODS_CHECK_NULL_VOID(ring);                                          \
        ring->head = 0;                                                        \
        ring->tail = 0;                                                        \
        ring->count = 0;                                                       \
        ring->capacity = SIZE;                                                 \
        ring->flags = flags;                                                   \
    }                                                                          \
                                                                               \
    static inline int nr_write_##T##_##SIZE(NanoRing_##T##_##SIZE* ring,      \
                                              T value) {                       \
        NANODS_CHECK_NULL(ring, NANODS_ERR_NULL);                              \
        NANODS_CHECK_FULL(ring->count, ring->capacity, NANODS_ERR_FULL);      \
        ring->data[ring->tail] = value;                                        \
        ring->tail = (ring->tail + 1) % ring->capacity;                        \
        ring->count++;                                                         \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nr_read_##T##_##SIZE(NanoRing_##T##_##SIZE* ring,       \
                                             T* out) {                         \
        NANODS_CHECK_NULL(ring, NANODS_ERR_NULL);                              \
        NANODS_CHECK_NULL(out, NANODS_ERR_NULL);                               \
        NANODS_CHECK_EMPTY(ring->count, NANODS_ERR_EMPTY);                    \
        *out = ring->data[ring->head];                                         \
        ring->head = (ring->head + 1) % ring->capacity;                        \
        ring->count--;                                                         \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nr_peek_##T##_##SIZE(const NanoRing_##T##_##SIZE* ring, \
                                             T* out) {                         \
        NANODS_CHECK_NULL(ring, NANODS_ERR_NULL);                              \
        NANODS_CHECK_NULL(out, NANODS_ERR_NULL);                               \
        NANODS_CHECK_EMPTY(ring->count, NANODS_ERR_EMPTY);                    \
        *out = ring->data[ring->head];                                         \
        return NANODS_OK;                                                      \
    }                                                                          \
                                                                               \
    static inline int nr_is_full_##T##_##SIZE(const NanoRing_##T##_##SIZE* ring) { \
        return ring ?  (ring->count == ring->capacity) : 1;                     \
    }                                                                          \
                                                                               \
    static inline int nr_is_empty_##T##_##SIZE(const NanoRing_##T##_##SIZE* ring) { \
        return ring ? (ring->count == 0) : 1;                                  \
    }                                                                          \
                                                                               \
    static inline size_t nr_size_##T##_##SIZE(const NanoRing_##T##_##SIZE* ring) { \
        return ring ? ring->count : 0;                                         \
    }                                                                          \
                                                                               \
    static inline size_t nr_capacity_##T##_##SIZE(const NanoRing_##T##_##SIZE* ring) { \
        return ring ? ring->capacity : 0;                                      \
    }                                                                          \
                                                                               \
    static inline void nr_clear_##T##_##SIZE(NanoRing_##T##_##SIZE* ring) {   \
        if (ring) {                                                            \
            if (ring->flags & NANODS_FLAG_SECURE) {                            \
                memset(ring->data, 0, sizeof(ring->data));                     \
            }                                                                  \
            ring->head = 0;                                                    \
            ring->tail = 0;                                                    \
            ring->count = 0;                                                   \
        }                                                                      \
    }

/* Pre-defined ring buffers */
NANODS_DEFINE_RING(int, 16)
NANODS_DEFINE_RING(int, 32)
NANODS_DEFINE_RING(int, 64)
NANODS_DEFINE_RING(int, 128)
NANODS_DEFINE_RING(int, 256)

NANODS_DEFINE_RING(char, 16)
NANODS_DEFINE_RING(char, 32)
NANODS_DEFINE_RING(char, 64)
NANODS_DEFINE_RING(char, 128)
NANODS_DEFINE_RING(char, 256)

typedef NanoRing_int_16 IntRing16;
typedef NanoRing_int_32 IntRing32;
typedef NanoRing_int_64 IntRing64;
typedef NanoRing_int_128 IntRing128;
typedef NanoRing_int_256 IntRing256;

typedef NanoRing_char_16 CharRing16;
typedef NanoRing_char_32 CharRing32;
typedef NanoRing_char_64 CharRing64;
typedef NanoRing_char_128 CharRing128;
typedef NanoRing_char_256 CharRing256;

/** @} */

#endif /* NANODS_RING_IMPL_H */
/* ==================== src/map_impl.h ==================== */
/**
 * @file map_impl.h
 * @brief String-keyed hash map with randomized seeding (Anti-DoS)
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
    uint32_t seed;
    uint8_t flags;
} NanoMap;

/**
 * FNV-1a hash with seed (Anti-DoS protection)
 */
static inline uint32_t nanods_fnv1a_hash_seeded(const char* key, uint32_t seed) {
    NANODS_CHECK_NULL(key, 0);
    uint32_t hash = 2166136261u ^ seed;
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
    map->seed = nanods_get_seed();
    map->flags = NANODS_FLAG_NONE;
}

static inline void nm_init_ex(NanoMap* map, uint8_t flags) {
    NANODS_CHECK_NULL_VOID(map);
    map->buckets = NULL;
    map->bucket_count = 0;
    map->size = 0;
    map->seed = nanods_get_seed();
    map->flags = flags;
}

static inline int nm_init_with_capacity(NanoMap* map, size_t bucket_count) {
    NANODS_CHECK_NULL(map, NANODS_ERR_NULL);
    if (bucket_count == 0) bucket_count = 16;
    size_t byte_size;
    if (NANODS_UNLIKELY(nanods_check_mul_overflow(bucket_count, sizeof(NanoMapEntry*), &byte_size)))
        return NANODS_ERR_OVERFLOW;
    map->buckets = (NanoMapEntry**)NANODS_MALLOC(byte_size);
    if (NANODS_UNLIKELY(!map->buckets)) return NANODS_ERR_NOMEM;
    memset(map->buckets, 0, byte_size);
    map->bucket_count = bucket_count;
    map->size = 0;
    map->seed = nanods_get_seed();
    map->flags = NANODS_FLAG_NONE;
    return NANODS_OK;
}

static inline int nm_init_with_capacity_ex(NanoMap* map, size_t bucket_count, uint8_t flags) {
    int err = nm_init_with_capacity(map, bucket_count);
    if (NANODS_LIKELY(err == NANODS_OK)) {
        map->flags = flags;
    }
    return err;
}

static inline NanoMapEntry* nm_find_entry(NanoMap* map, const char* key, size_t* out_bucket_idx) {
    NANODS_CHECK_NULL(map, NULL);
    NANODS_CHECK_NULL(key, NULL);
    if (map->bucket_count == 0) return NULL;
    uint32_t hash = nanods_fnv1a_hash_seeded(key, map->seed);
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
        if (NANODS_UNLIKELY(err != NANODS_OK)) return err;
    }
    size_t bucket_idx;
    NanoMapEntry* existing = nm_find_entry(map, key, &bucket_idx);
    if (existing) {
        existing->value = value;
        return NANODS_OK;
    }
    NanoMapEntry* new_entry = (NanoMapEntry*)NANODS_MALLOC(sizeof(NanoMapEntry));
    if (NANODS_UNLIKELY(!new_entry)) return NANODS_ERR_NOMEM;
    new_entry->key = (char*)NANODS_MALLOC(strlen(key) + 1);
    if (NANODS_UNLIKELY(!new_entry->key)) {
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
    uint32_t hash = nanods_fnv1a_hash_seeded(key, map->seed);
    size_t bucket_idx = hash % map->bucket_count;
    NanoMapEntry** indirect = &map->buckets[bucket_idx];
    int secure = (map->flags & NANODS_FLAG_SECURE);
    while (*indirect) {
        NanoMapEntry* entry = *indirect;
        if (strcmp(entry->key, key) == 0) {
            *indirect = entry->next;
            if (secure) {
                size_t key_len = strlen(entry->key);
                memset(entry->key, 0, key_len);
            }
            NANODS_FREE(entry->key);
            if (secure) {
                nanods_secure_free(entry, sizeof(NanoMapEntry));
            } else {
                NANODS_FREE(entry);
            }
            map->size--;
            return NANODS_OK;
        }
        indirect = &entry->next;
    }
    return NANODS_ERR_NOTFOUND;
}

static inline size_t nm_size(const NanoMap* map) {
    return map ? map->size : 0;
}

static inline int nm_empty(const NanoMap* map) {
    return map ? (map->size == 0) : 1;
}

static inline void nm_clear(NanoMap* map) {
    if (!map) return;
    int secure = (map->flags & NANODS_FLAG_SECURE);
    for (size_t i = 0; i < map->bucket_count; i++) {
        NanoMapEntry* entry = map->buckets[i];
        while (entry) {
            NanoMapEntry* next = entry->next;
            if (secure) {
                size_t key_len = strlen(entry->key);
                memset(entry->key, 0, key_len);
            }
            NANODS_FREE(entry->key);
            if (secure) {
                nanods_secure_free(entry, sizeof(NanoMapEntry));
            } else {
                NANODS_FREE(entry);
            }
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
            nanods_secure_free(entry, sizeof(NanoMapEntry));
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

#endif /* NANODS_MAP_IMPL_H */
/* ==================== src/iterator_impl.h ==================== */
/**
 * @file iterator_impl.h
 * @brief Universal iterator for all NanoDS containers
 */

/**
 * @defgroup NanoIterator Universal Iterator
 * @{
 */

typedef enum {
    NANODS_ITER_VECTOR,
    NANODS_ITER_LIST,
    NANODS_ITER_LIST2,
    NANODS_ITER_MAP
} NanoIterType;

typedef struct {
    NanoIterType type;
    void* container;
    void* ptr;           /* Current element pointer */
    void* internal;      /* Internal state (node, bucket, etc.) */
    size_t index;        /* Current index (for vectors) */
    int finished;        /* 1 if iteration complete */
} NanoIter;

/* Vector Iterator */
#define NANODS_VECTOR_ITER_INIT(T)                                             \
    static inline NanoIter nv_iter_##T(NanoVector_##T* vec) {                 \
        NanoIter it;                                                           \
        it.type = NANODS_ITER_VECTOR;                                          \
        it.container = vec;                                                    \
        it.index = 0;                                                          \
        it.ptr = (vec && vec->size > 0) ? &vec->data[0] : NULL;               \
        it.internal = NULL;                                                    \
        it.finished = (! vec || vec->size == 0);                                \
        return it;                                                             \
    }                                                                          \
                                                                               \
    static inline int nv_iter_next_##T(NanoIter* it) {                        \
        if (! it || it->finished) return 0;                                     \
        NanoVector_##T* vec = (NanoVector_##T*)it->container;                 \
        it->index++;                                                           \
        if (it->index >= vec->size) {                                          \
            it->finished = 1;                                                  \
            it->ptr = NULL;                                                    \
            return 0;                                                          \
        }                                                                      \
        it->ptr = &vec->data[it->index];                                      \
        return 1;                                                              \
    }

/* List Iterator */
#define NANODS_LIST_ITER_INIT(T)                                               \
    static inline NanoIter nl_iter_##T(NanoList_##T* list) {                  \
        NanoIter it;                                                           \
        it.type = NANODS_ITER_LIST;                                            \
        it.container = list;                                                   \
        it.index = 0;                                                          \
        it.internal = (list && list->head) ? list->head : NULL;               \
        it.ptr = it.internal ?  &((NanoListNode_##T*)it.internal)->data : NULL; \
        it.finished = (!list || ! list->head);                                  \
        return it;                                                             \
    }                                                                          \
                                                                               \
    static inline int nl_iter_next_##T(NanoIter* it) {                        \
        if (!it || it->finished) return 0;                                     \
        NanoListNode_##T* node = (NanoListNode_##T*)it->internal;             \
        if (!node || ! node->next) {                                            \
            it->finished = 1;                                                  \
            it->ptr = NULL;                                                    \
            it->internal = NULL;                                               \
            return 0;                                                          \
        }                                                                      \
        it->internal = node->next;                                             \
        it->ptr = &((NanoListNode_##T*)it->internal)->data;                   \
        it->index++;                                                           \
        return 1;                                                              \
    }

/* List2 Iterator */
#define NANODS_LIST2_ITER_INIT(T)                                              \
    static inline NanoIter nl2_iter_##T(NanoList2_##T* list) {                \
        NanoIter it;                                                           \
        it.type = NANODS_ITER_LIST2;                                           \
        it.container = list;                                                   \
        it.index = 0;                                                          \
        it.internal = (list && list->head) ? list->head : NULL;               \
        it.ptr = it.internal ? &((NanoList2Node_##T*)it.internal)->data : NULL; \
        it.finished = (!list || !list->head);                                  \
        return it;                                                             \
    }                                                                          \
                                                                               \
    static inline int nl2_iter_next_##T(NanoIter* it) {                       \
        if (!it || it->finished) return 0;                                     \
        NanoList2Node_##T* node = (NanoList2Node_##T*)it->internal;           \
        if (!node || !node->next) {                                            \
            it->finished = 1;                                                  \
            it->ptr = NULL;                                                    \
            it->internal = NULL;                                               \
            return 0;                                                          \
        }                                                                      \
        it->internal = node->next;                                             \
        it->ptr = &((NanoList2Node_##T*)it->internal)->data;                  \
        it->index++;                                                           \
        return 1;                                                              \
    }

/* Define iterators for built-in types */
NANODS_VECTOR_ITER_INIT(int)
NANODS_VECTOR_ITER_INIT(float)
NANODS_VECTOR_ITER_INIT(double)
NANODS_VECTOR_ITER_INIT(char)

NANODS_LIST_ITER_INIT(int)
NANODS_LIST_ITER_INIT(float)
NANODS_LIST_ITER_INIT(double)
NANODS_LIST_ITER_INIT(char)

NANODS_LIST2_ITER_INIT(int)
NANODS_LIST2_ITER_INIT(float)
NANODS_LIST2_ITER_INIT(double)
NANODS_LIST2_ITER_INIT(char)

/* Map Iterator (already has nm_iter_init/nm_iter_next) */
static inline NanoIter nm_iter(NanoMap* map) {
    NanoIter it;
    it.type = NANODS_ITER_MAP;
    it.container = map;
    it.index = 0;
    it.ptr = NULL;
    it.internal = NANODS_MALLOC(sizeof(NanoMapIterator));
    if (it.internal) {
        nm_iter_init((NanoMapIterator*)it.internal, map);
    }
    it.finished = (! map || map->size == 0);
    return it;
}

static inline int nm_iter_has_next(NanoIter* it) {
    if (!it || it->finished || ! it->internal) return 0;
    NanoMapIterator* map_it = (NanoMapIterator*)it->internal;
    return map_it->current != NULL;
}

static inline void nm_iter_free(NanoIter* it) {
    if (it && it->internal && it->type == NANODS_ITER_MAP) {
        NANODS_FREE(it->internal);
        it->internal = NULL;
    }
}

/* Generic iterator check */
static inline int nanods_iter_has_next(NanoIter* it) {
    if (!it) return 0;
    switch (it->type) {
        case NANODS_ITER_VECTOR: 
        case NANODS_ITER_LIST:
        case NANODS_ITER_LIST2:
            return ! it->finished;
        case NANODS_ITER_MAP: 
            return nm_iter_has_next(it);
        default:
            return 0;
    }
}

/** @} */

#endif /* NANODS_ITERATOR_IMPL_H */
#ifdef __cplusplus
}
#endif

#endif /* NANODS_H */

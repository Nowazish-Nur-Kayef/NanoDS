/**
 * @file core.h
 * @brief Core utilities, allocator interface, and safety macros
 */

#ifndef NANODS_CORE_H
#define NANODS_CORE_H

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
typedef struct {
    void* (*malloc_fn)(size_t size);
    void* (*realloc_fn)(void* ptr, size_t size);
    void  (*free_fn)(void* ptr);
} NanoAllocator;

void nanods_set_allocator(NanoAllocator* allocator);
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

#endif /* NANODS_CORE_H */

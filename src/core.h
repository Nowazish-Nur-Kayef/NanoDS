/**
 * @file core.h
 * @brief Core utilities, allocator interface, security, and flags
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
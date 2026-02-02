/**
 * @file ring_impl.h
 * @brief Fixed-size circular buffer implementation
 */

#ifndef NANODS_RING_IMPL_H
#define NANODS_RING_IMPL_H

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
/**
 * @file vector_impl.h
 * @brief Generic dynamic array implementation with functional operations
 */

#ifndef NANODS_VECTOR_IMPL_H
#define NANODS_VECTOR_IMPL_H

/**
 * @defgroup NanoVector Generic Dynamic Array
 * @{
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
        return vec ?  (vec->size == 0) : 1;                                     \
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
            if (err != NANODS_OK) {                                            \
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
                if (err != NANODS_OK) {                                        \
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

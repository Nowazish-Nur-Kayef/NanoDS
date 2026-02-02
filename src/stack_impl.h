/**
 * @file stack_impl.h
 * @brief LIFO Stack implementation (composition over vector)
 */

#ifndef NANODS_STACK_IMPL_H
#define NANODS_STACK_IMPL_H

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
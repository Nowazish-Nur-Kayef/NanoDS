/**
 * @file list2_impl.h
 * @brief Doubly linked list implementation
 */

#ifndef NANODS_LIST2_IMPL_H
#define NANODS_LIST2_IMPL_H

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
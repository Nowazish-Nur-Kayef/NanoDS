/**
 * @file list_impl.h
 * @brief Singly linked list implementation
 */

#ifndef NANODS_LIST_IMPL_H
#define NANODS_LIST_IMPL_H

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

#endif /* NANODS_LIST_IMPL_H */

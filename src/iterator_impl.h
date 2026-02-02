/**
 * @file iterator_impl.h
 * @brief Universal iterator for all NanoDS containers
 */

#ifndef NANODS_ITERATOR_IMPL_H
#define NANODS_ITERATOR_IMPL_H

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
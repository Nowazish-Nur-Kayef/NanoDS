/**
 * @file map_impl.h
 * @brief String-keyed hash map with randomized seeding (Anti-DoS)
 */

#ifndef NANODS_MAP_IMPL_H
#define NANODS_MAP_IMPL_H

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
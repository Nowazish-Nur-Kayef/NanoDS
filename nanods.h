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

/* Core definitions and utilities */
#include "src/core.h"

/* Data structure implementations */
#include "src/vector_impl.h"
#include "src/stack_impl.h"
#include "src/list_impl.h"
#include "src/list2_impl.h"    /* NEW: Doubly linked list */
#include "src/ring_impl.h"     /* NEW:  Circular buffer */
#include "src/map_impl.h"
#include "src/iterator_impl.h" /* NEW: Universal iterator */

#ifdef __cplusplus
}
#endif

#endif /* NANODS_H */
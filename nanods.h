/**
 * @file nanods. h
 * @brief NanoDS - Production-Grade Nano Data Structures
 * @version 0.1.1
 * @author Nowazish-Nur-Kayef
 * @date 2025
 * @license MIT / Public Domain
 * 
 * =============================================================================
 * NanoDS - Nano Data Structures
 * A production-ready, memory-safe, single-header C library for generic,
 * type-safe data structures with custom allocator support.
 * =============================================================================
 */

#ifndef NANODS_H
#define NANODS_H

#define NANODS_VERSION_MAJOR 0
#define NANODS_VERSION_MINOR 1
#define NANODS_VERSION_PATCH 1
#define NANODS_VERSION "0.1.1"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Core definitions and utilities */
#include "src/core.h"

/* Data structure implementations */
#include "src/vector_impl.h"
#include "src/stack_impl.h"
#include "src/list_impl.h"
#include "src/map_impl.h"

#ifdef __cplusplus
}
#endif

#endif /* NANODS_H */

# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Doubly linked list implementation
- Queue/Deque data structures
- Binary search tree
- Performance optimization guide
- More comprehensive examples

---

## [0.1.0] - 2025-12-29

### Added

#### Core Features
- **NanoVector**: Generic type-safe dynamic arrays
  - Automatic capacity doubling with exponential growth
  - Type-safe macros for compile-time safety
  - Pre-defined types: `IntVector`, `FloatVector`, `DoubleVector`, `CharVector`
  - API: `nv_init`, `nv_push`, `nv_get`, `nv_set`, `nv_pop`, `nv_size`, `nv_empty`, `nv_clear`, `nv_free`
  
- **NanoStack**:  LIFO stack using composition over NanoVector
  - Zero-copy implementation leveraging existing vector code
  - API: `ns_init`, `ns_push`, `ns_pop`, `ns_peek`, `ns_size`, `ns_empty`, `ns_free`
  
- **NanoList**:  Singly linked list
  - O(1) push/pop operations at front and back
  - Efficient memory usage with per-node allocation
  - API: `nl_init`, `nl_push_front`, `nl_push_back`, `nl_pop_front`, `nl_size`, `nl_empty`, `nl_free`
  
- **NanoMap**: String-keyed hash map
  - FNV-1a hash algorithm for collision resistance
  - Separate chaining for collision resolution
  - Dynamic bucket allocation
  - Iterator support for traversal
  - API: `nm_init`, `nm_set`, `nm_get`, `nm_has`, `nm_remove`, `nm_size`, `nm_empty`, `nm_clear`, `nm_free`

#### Safety Features
- **Hybrid Safety System**: Dual-mode error checking
  - **Debug Mode** (default): Uses `assert()` for development-time error detection
  - **Hard Safety Mode**: Define `NANODS_HARD_SAFETY` for production runtime checks
  - Comprehensive error codes:  `NANODS_OK`, `NANODS_ERR_NOMEM`, `NANODS_ERR_BOUNDS`, `NANODS_ERR_EMPTY`, `NANODS_ERR_OVERFLOW`, `NANODS_ERR_NOTFOUND`, `NANODS_ERR_NULL`
  
- **Integer Overflow Protection**: All size calculations checked against `SIZE_MAX`
  - `nanods_check_mul_overflow()` for multiplication
  - `nanods_check_add_overflow()` for addition
  
- **Memory Safety**:
  - Safe reallocation wrappers to prevent leaks on `realloc()` failure
  - Bounds checking on all array access operations
  - NULL pointer validation on all public APIs
  - Secure free with memory zeroing:  `nv_secure_free`, `nm_secure_free`

#### Custom Allocator Support
- **Allocator Interface** for embedded systems and custom memory management
  - `NanoAllocator` struct with function pointers:  `malloc_fn`, `realloc_fn`, `free_fn`
  - `nanods_set_allocator()` to override default system allocator
  - `nanods_get_allocator()` to query current allocator
  - All internal allocations go through allocator interface

#### Documentation
- **Doxygen-style API documentation** in header file
  - Full function documentation with `@brief`, `@param`, `@return`
  - Grouped documentation with `@defgroup`
  - Usage examples in header comments
  
- **Comprehensive README. md**: 
  - Quick start guide
  - API reference tables
  - Security specifications
  - Build instructions
  - Real-world usage examples

#### Testing & CI/CD
- **GitHub Actions CI/CD Pipeline** (`.github/workflows/ci.yml`):
  - Multi-platform builds:  Ubuntu, macOS, Windows
  - Multiple compiler versions: GCC 9-12, Clang 12-14
  - Automated memory leak detection with Valgrind
  - Hard safety mode testing
  - Code coverage reporting
  - Performance regression checks
  
- **Comprehensive Test Suite** (`test. c`):
  - Unit tests for all data structures
  - Error handling tests
  - Edge case validation
  - Custom struct support tests
  - 100% Valgrind verified (zero memory leaks)

- **Performance Benchmark Suite** (`benchmark.c`):
  - 1 million operations per data structure
  - Cross-platform high-resolution timing
  - Ops/second and per-operation latency metrics
  - Baseline performance tracking in CI

#### Build System
- **Cross-Platform Makefile**:
  - Automatic OS detection (Linux, macOS, Windows)
  - Multiple build targets: `debug`, `release`, `safe`, `test`, `benchmark`
  - Valgrind integration for memory checking
  - Clean and help targets

### Security

- **Memory Safety**:
  - All allocations protected against integer overflow
  - Safe reallocation prevents memory leaks
  - Bounds checking on every array access
  - NULL pointer checks on all APIs
  
- **Production Hardening**:
  - Hard safety mode for runtime error checking without asserts
  - Secure memory zeroing before deallocation
  - FNV-1a hashing resistant to DoS attacks
  
- **Validation**:
  - Valgrind verified:  zero memory leaks, zero errors
  - Compiler warning-free with `-Wall -Wextra -Wpedantic`
  - Tested on GCC, Clang, MSVC

### Performance

- **Vector Operations**:
  - Push:  ~300+ million ops/sec (O3 optimization)
  - Get: O(1) constant time
  - Growth strategy: exponential doubling
  
- **Stack Operations**:
  - Push/Pop: ~900+ million ops/sec
  - Peek: O(1) constant time
  
- **List Operations**:
  - Push front/back: O(1) constant time
  - Pop front:  O(1) constant time
  
- **Map Operations**:
  - Set/Get: O(1) average case
  - FNV-1a hash: optimized for speed and distribution

### Technical Details

- **Language Standard**: C11 (`-std=c11`)
- **Compiler Support**: GCC 4.9+, Clang 3.4+, MSVC 2015+
- **Platform Support**: Linux, macOS, Windows, embedded systems
- **Dependencies**: None (only standard C library)
- **Header Size**: Single file, ~750 lines
- **License**: MIT / Public Domain (user choice)

### Repository Structure

```
NanoDS/
├── nanods.h           # Main library (header-only)
├── test.c             # Comprehensive test suite
├── benchmark.c        # Performance benchmarks
├── Makefile           # Cross-platform build system
├── README.md          # Full documentation
├── CHANGELOG.md       # This file
├── LICENSE            # MIT License
└── . github/
    └── workflows/
        └── ci.yml     # GitHub Actions CI/CD
```

---

## Version History

### Semantic Versioning Guide

NanoDS follows [Semantic Versioning 2.0.0](https://semver.org/):

- **MAJOR** version (X.0.0): Incompatible API changes
- **MINOR** version (0.X.0): New functionality (backward-compatible)
- **PATCH** version (0.0.X): Bug fixes (backward-compatible)

### Version 0.1.0 Notes

This is the **initial production release** of NanoDS.  While versioned as `0.1.0` to indicate it's a first release, it includes: 

✅ Production-ready code quality  
✅ Comprehensive test coverage  
✅ Full CI/CD pipeline  
✅ Complete documentation  
✅ Memory safety guarantees  
✅ Cross-platform support  

**API Stability**: The API is considered stable.  Future 0.x releases will maintain backward compatibility except where explicitly noted.

---

## Migration Guide

### Upgrading from Pre-release Versions

If you used NanoDS before v0.1.0, note the following **breaking changes**:

#### API Changes

**Old (pre-v0.1.0):**
```c
int value = nv_get_int(&vec, 0);  // Returns value directly
```

**New (v0.1.0):**
```c
int value;
int err = nv_get_int(&vec, 0, &value);  // Returns error code
if (err != NANODS_OK) {
    // Handle error
}
```

**Reason**:  Consistent error handling across all functions.

#### Error Handling

All functions now return error codes instead of values: 

| Function Type | Return Value |
|--------------|--------------|
| `nv_push`, `nv_set`, `nv_pop` | `int` (error code) |
| `nv_get` | Error code, value via output parameter |
| `nv_size`, `nv_empty` | `size_t` / `int` (no error code) |
| `nv_init`, `nv_free`, `nv_clear` | `void` (no return) |

---

## Links

- **Repository**: https://github.com/Nowazish-Nur-Kayef/NanoDS
- **Issues**: https://github.com/Nowazish-Nur-Kayef/NanoDS/issues
- **Discussions**: https://github.com/Nowazish-Nur-Kayef/NanoDS/discussions
- **Releases**: https://github.com/Nowazish-Nur-Kayef/NanoDS/releases

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on contributing to NanoDS.

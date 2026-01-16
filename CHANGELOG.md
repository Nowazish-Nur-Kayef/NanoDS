# Changelog

All notable changes to this project will be documented in this file. 

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Doubly linked list implementation
- Queue/Deque data structures
- Binary search tree
- More comprehensive examples

---

## [0.1.1] - 2026-01-16

### Added - The "Ecosystem" Update

#### Modularity & Organization
- **Split-Header Pattern**:  Reorganized codebase into modular components
  - `src/core. h` - Core utilities and allocator interface
  - `src/vector_impl.h` - Vector implementation
  - `src/stack_impl.h` - Stack implementation
  - `src/list_impl.h` - List implementation
  - `src/map_impl.h` - Map implementation
- **Single-File Bundler**: `scripts/bundle.py` to merge split headers into one file
  - Maintains single-header distribution model
  - Easier maintenance with modular source

#### Functional Programming Features
- **`nv_map_TYPE()`**: Apply function to every element in vector
  - Example:  `nv_map_int(&vec, &result, double_value)`
- **`nv_filter_TYPE()`**: Create new vector based on predicate
  - Example:  `nv_filter_int(&vec, &result, is_even)`
- Enables functional-style C programming (map/filter/reduce patterns)

#### Performance & Benchmarking
- **Comprehensive Benchmark Suite**: 
  - `benchmarks/bench_vector.c` - Vector performance metrics
  - `benchmarks/bench_map.c` - Hash map performance across scales (1K-1M entries)
  - `benchmarks/bench_comparison.c` - NanoDS vs naive implementation
  - `benchmarks/run_benchmarks.sh` - Automated benchmark runner
- **Empirical Performance Data**: 
  - CSV export for performance tracking
  - Cross-platform timing (Windows, macOS, Linux)
  - Throughput and latency metrics

#### Package Manager Integration
- **CMake Support**: Full CMakeLists.txt for modern C/C++ projects
  - `add_subdirectory()` support
  - `find_package()` integration
  - Install targets for system-wide installation
  - Configurable options (tests, benchmarks, examples, hard safety)
- **pkg-config Support**: `nanods.pc.in` template
  - Easy linking on Linux:  `pkg-config --cflags --libs nanods`
  - Integration with Autotools and other build systems

#### Real-World Examples
- **`examples/word_frequency.c`**: Word counter using NanoMap
  - Demonstrates hash map usage
  - Text processing and string manipulation
  - Iterator patterns
- **`examples/command_history.c`**: CLI history using NanoVector
  - Demonstrates vector usage
  - Search and filtering
  - Sequential data management

### Changed

- **Repository Structure**:  Reorganized for better maintainability
  - Modular source files in `src/`
  - Dedicated `benchmarks/` directory
  - Dedicated `examples/` directory
  - Scripts in `scripts/` directory
- **Makefile**: Enhanced with new targets
  - `make examples` - Build all examples
  - `make benchmarks` - Build all benchmarks
  - `make bundle` - Create single-file distribution
  - `make run-examples` - Run all examples
  - `make run-benchmarks` - Run all benchmarks
  - `make install` - System-wide installation

### Improved

- **Developer Experience**: 
  - Easier to navigate codebase with split headers
  - Clear separation of concerns
  - Better IDE support with modular files
- **Distribution**:
  - Both modular (for development) and single-file (for distribution)
  - CMake integration for professional projects
  - pkg-config for traditional Linux workflows
- **Documentation**:
  - Real-world examples show practical usage
  - Benchmark data provides performance transparency
  - CMake examples for integration

### Technical Details

- **Build System**: CMake 3.10+ support added
- **Bundling**: Python 3 script for single-file generation
- **Benchmarking**: High-resolution cross-platform timing
- **Examples**: Production-quality demonstration code

---

## [0.1.0] - 2025-12-29

### Added - Initial Production Release

#### Core Features
- **NanoVector**: Generic type-safe dynamic arrays
  - Automatic capacity doubling
  - Type-safe macros
  - Pre-defined types: `IntVector`, `FloatVector`, `DoubleVector`, `CharVector`
  
- **NanoStack**:  LIFO stack using composition over NanoVector
  - Zero-copy implementation
  
- **NanoList**: Singly linked list
  - O(1) push/pop operations
  
- **NanoMap**: String-keyed hash map
  - FNV-1a hash algorithm
  - Separate chaining
  - Iterator support

#### Safety Features
- **Hybrid Safety System**: Dual-mode error checking
  - Debug Mode:  `assert()` for development
  - Hard Safety Mode: Runtime checks with error codes
  
- **Integer Overflow Protection**
- **Memory Safety**: Bounds checking, NULL validation, secure free
- **Comprehensive Error Codes**

#### Custom Allocator Support
- **Allocator Interface** for embedded systems
- All allocations go through configurable allocator

#### Testing & CI/CD
- **GitHub Actions CI/CD Pipeline**
- **Comprehensive Test Suite**
- **Performance Benchmark Suite**
- **Cross-Platform Makefile**

### Security
- Memory safety guarantees
- Integer overflow protection
- FNV-1a hashing resistant to DoS
- Valgrind verified (zero leaks)

### Performance
- Vector Push: ~340M ops/sec
- Stack Push/Pop: ~920M ops/sec
- List Push: ~53M ops/sec
- Map Set/Get: ~500K ops/sec

## Migration Guide

### Upgrading from v0.1.0 to v0.1.1

No breaking API changes.  New features are additions only.

**New Features Available:**
```c
// Functional operations (NEW in v0.1.1)
int double_value(int x) { return x * 2; }
nv_map_int(&vec, &result, double_value);

int is_even(int x) { return x % 2 == 0; }
nv_filter_int(&vec, &result, is_even);
```

**Modular Headers** (optional, for development):
```c
// Old way (still works)
#define NANODS_IMPLEMENTATION
#include "nanods. h"

// New way (for modular development)
#include "nanods. h"  // Includes all src/*. h automatically
```

**CMake Integration** (new):
```cmake
add_subdirectory(NanoDS)
target_link_libraries(myapp PRIVATE nanods)
```

---

## Links

- **Repository**: https://github.com/Nowazish-Nur-Kayef/NanoDS
- **Issues**: https://github.com/Nowazish-Nur-Kayef/NanoDS/issues
- **Releases**: https://github.com/Nowazish-Nur-Kayef/NanoDS/releases

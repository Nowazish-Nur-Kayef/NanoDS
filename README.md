# ⚡ NanoDS (Nano Data Structures)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Language:  C](https://img.shields.io/badge/Language-C-00599C.svg?style=flat&logo=c)
![C Standard](https://img.shields.io/badge/C-C11-blue.svg)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/Nowazish-Nur-Kayef/NanoDS?color=blue)
![Memory Safe](https://img.shields.io/badge/Memory-Safe-green.svg)
![CI Status](https://img.shields.io/github/actions/workflow/status/Nowazish-Nur-Kayef/NanoDS/ci.yml?branch=main)

**NanoDS** is a production-ready, memory-safe, single-header C library providing generic, type-safe data structures with zero dependencies and custom allocator support.

---

## 🛠 Why NanoDS?

Standard C lacks built-in generic containers.  Developers often resort to unsafe `void*` casts or manual memory management. **NanoDS** solves this by using C macros to provide **compile-time type safety**, **runtime error checking**, and **built-in security guards**.

### ✨ Key Features

* 📦 **Single-Header** - Drop `nanods.h` into your project.  No build systems required
* 🔒 **Type-Safe** - Generic dynamic arrays, stacks, lists, and maps for any data type
* 🛡️ **Hybrid Safety System** - Assert-based debugging + optional hard runtime checks
* 🗺️ **Secure Hash Maps** - FNV-1a algorithm with collision resistance
* 🔧 **Custom Allocators** - Full allocator interface for embedded systems
* ✅ **Zero Memory Leaks** - Valgrind verified, 100% clean
* 🚀 **Zero Dependencies** - Only requires standard C library
* 🌍 **Cross-Platform** - Linux, macOS, Windows
* 📊 **CI/CD Ready** - Automated testing on every commit

---

## 🚀 Quick Start

### Installation

**Option 1: Direct Download**
```bash
wget https://raw.githubusercontent.com/Nowazish-Nur-Kayef/NanoDS/main/nanods.h
```

**Option 2: Clone Repository**
```bash
git clone https://github.com/Nowazish-Nur-Kayef/NanoDS.git
cd NanoDS
```

### Basic Usage

#### 1. Include the Library

In **exactly one** C file, define `NANODS_IMPLEMENTATION` before including:  

```c
#define NANODS_IMPLEMENTATION
#include "nanods.h"
```

In all other files, just include normally:

```c
#include "nanods.h"
```

#### 2. Using Vectors

```c
#include <stdio.h>
#define NANODS_IMPLEMENTATION
#include "nanods.h"

int main(void) {
    // Create an integer vector
    IntVector vec;
    nv_init_int(&vec);
    
    // Add elements
    nv_push_int(&vec, 42);
    nv_push_int(&vec, 100);
    nv_push_int(&vec, 256);
    
    // Access elements
    int value;
    nv_get_int(&vec, 0, &value);
    printf("First element: %d\n", value);
    printf("Vector size: %zu\n", nv_size_int(&vec));
    
    // Iterate
    for (size_t i = 0; i < nv_size_int(&vec); i++) {
        int val;
        nv_get_int(&vec, i, &val);
        printf("%d ", val);
    }
    printf("\n");
    
    // Clean up
    nv_free_int(&vec);
    return 0;
}
```

#### 3. Using Stacks

```c
#include <stdio.h>
#define NANODS_IMPLEMENTATION
#include "nanods.h"

int main(void) {
    IntStack stack;
    ns_init_int(&stack);
    
    // Push elements
    for (int i = 1; i <= 5; i++) {
        ns_push_int(&stack, i * 10);
    }
    
    // Peek at top
    int top;
    ns_peek_int(&stack, &top);
    printf("Top: %d\n", top);
    
    // Pop all elements
    printf("Popping:  ");
    while (!ns_empty_int(&stack)) {
        int val;
        ns_pop_int(&stack, &val);
        printf("%d ", val);
    }
    printf("\n");
    
    ns_free_int(&stack);
    return 0;
}
```

#### 4. Using Linked Lists

```c
#include <stdio.h>
#define NANODS_IMPLEMENTATION
#include "nanods.h"

int main(void) {
    IntList list;
    nl_init_int(&list);
    
    // Add elements
    nl_push_back_int(&list, 10);
    nl_push_back_int(&list, 20);
    nl_push_front_int(&list, 5);  // Add to front
    
    printf("List size: %zu\n", nl_size_int(&list));
    
    // Pop from front
    int value;
    nl_pop_front_int(&list, &value);
    printf("Popped:  %d\n", value);
    
    nl_free_int(&list);
    return 0;
}
```

#### 5. Using Hash Maps

```c
#include <stdio.h>
#define NANODS_IMPLEMENTATION
#include "nanods.h"

int main(void) {
    NanoMap config;
    nm_init(&config);
    
    // Store configuration
    int port = 8080;
    int max_conn = 100;
    
    nm_set(&config, "port", &port);
    nm_set(&config, "max_connections", &max_conn);
    
    // Retrieve values
    int* result = (int*)nm_get(&config, "port");
    if (result) {
        printf("Server port: %d\n", *result);
    }
    
    // Check existence
    if (nm_has(&config, "port")) {
        printf("Port is configured\n");
    }
    
    // Remove entry
    nm_remove(&config, "port");
    
    nm_free(&config);
    return 0;
}
```

#### 6. Custom Data Types

```c
#define NANODS_IMPLEMENTATION
#include "nanods.h"
#include <stdio.h>

typedef struct {
    char name[50];
    int age;
    float salary;
} Employee;

// Define vector for custom type
NANODS_DEFINE_VECTOR(Employee)

int main(void) {
    NanoVector_Employee employees;
    nv_init_Employee(&employees);
    
    Employee e1 = {"Alice Johnson", 28, 75000.0f};
    Employee e2 = {"Bob Smith", 35, 85000.0f};
    
    nv_push_Employee(&employees, e1);
    nv_push_Employee(&employees, e2);
    
    // Access
    Employee emp;
    nv_get_Employee(&employees, 0, &emp);
    printf("%s: Age %d, Salary $%.2f\n", emp. name, emp.age, emp. salary);
    
    nv_free_Employee(&employees);
    return 0;
}
```

---

## 🔨 Building

### Using Make (Recommended)

```bash
# Build test suite
make

# Run tests
make run

# Build with debug symbols
make debug

# Build with hard safety mode
make safe

# Run memory leak check (Linux/macOS)
make valgrind

# Run performance benchmarks
make run-benchmark

# Clean build files
make clean

# See all options
make help
```

### Manual Compilation

```bash
# Compile test suite
gcc -std=c11 -Wall -Wextra test.c -o nanods_test

# Run
./nanods_test

# Compile your own code
gcc -std=c11 -Wall -Wextra your_code.c -o your_program
```

### Requirements

- **C Compiler**: GCC 4.9+, Clang 3.4+, MSVC 2015+
- **C Standard**: C11 or later
- **OS**: Linux, macOS, Windows (cross-platform)

---

## 🛡️ Safety Features

### Hybrid Safety System

NanoDS provides two safety modes:

#### Debug Mode (Default)
```c
#define NANODS_IMPLEMENTATION
#include "nanods.h"
// Uses assert() for development-time error detection
```

#### Hard Safety Mode (Production)
```c
#define NANODS_HARD_SAFETY
#define NANODS_IMPLEMENTATION
#include "nanods.h"
// Runtime error checking without asserts
// Functions return error codes instead of asserting
```

### Security Specifications

| Feature | Protection | Implementation |
|---------|------------|----------------|
| **Overflow Check** | Integer overflow prevention | Validates `a * b < SIZE_MAX` before allocation |
| **Bounds Checking** | Out-of-bounds protection | Assert or error code on invalid indices |
| **Safe Realloc** | Memory leak prevention | Temporary pointer before overwriting |
| **Memory Sanitization** | Data exposure prevention | `nv_secure_free()` zeros memory with `memset()` |
| **Hash Collision** | DoS attack resistance | FNV-1a algorithm with good distribution |
| **NULL Safety** | Crash prevention | All pointers validated before use |

### Error Codes

```c
typedef enum {
    NANODS_OK = 0,           // Success
    NANODS_ERR_NOMEM = -1,   // Out of memory
    NANODS_ERR_BOUNDS = -2,  // Index out of bounds
    NANODS_ERR_EMPTY = -3,   // Container is empty
    NANODS_ERR_OVERFLOW = -4,// Integer overflow detected
    NANODS_ERR_NOTFOUND = -5,// Key not found (maps)
    NANODS_ERR_NULL = -6     // NULL pointer passed
} NanoDSError;
```

### Security Example

```c
// Secure handling of sensitive data
IntVector passwords;
nv_init_int(&passwords);
nv_push_int(&passwords, 12345);
nv_push_int(&passwords, 67890);

// Zero memory before freeing
nv_secure_free_int(&passwords);  // Memory is overwritten with 0x00
```

---

## 🔧 Custom Allocators

For embedded systems or custom memory management: 

```c
#define NANODS_IMPLEMENTATION
#include "nanods.h"

// Custom allocator functions
void* my_malloc(size_t size) {
    // Your custom allocation logic
    return custom_alloc(size);
}

void* my_realloc(void* ptr, size_t size) {
    // Your custom reallocation logic
    return custom_realloc(ptr, size);
}

void my_free(void* ptr) {
    // Your custom deallocation logic
    custom_free(ptr);
}

int main(void) {
    // Set custom allocator
    NanoAllocator custom = {
        .malloc_fn = my_malloc,
        . realloc_fn = my_realloc,
        .free_fn = my_free
    };
    nanods_set_allocator(&custom);
    
    // All NanoDS operations now use custom allocator
    IntVector vec;
    nv_init_int(&vec);
    nv_push_int(&vec, 42);
    nv_free_int(&vec);
    
    return 0;
}
```

---

## 📊 API Reference

### Vector Operations

| Function | Description | Returns |
|----------|-------------|---------|
| `nv_init_TYPE(&vec)` | Initialize empty vector | `void` |
| `nv_push_TYPE(&vec, value)` | Add element to end | `int` (error code) |
| `nv_get_TYPE(&vec, index, &out)` | Get element at index | `int` (error code) |
| `nv_set_TYPE(&vec, index, value)` | Set element at index | `int` (error code) |
| `nv_pop_TYPE(&vec, &out)` | Remove last element | `int` (error code) |
| `nv_reserve_TYPE(&vec, capacity)` | Pre-allocate capacity | `int` (error code) |
| `nv_size_TYPE(&vec)` | Get number of elements | `size_t` |
| `nv_empty_TYPE(&vec)` | Check if empty | `int` (1 if empty) |
| `nv_clear_TYPE(&vec)` | Remove all elements | `void` |
| `nv_free_TYPE(&vec)` | Free memory | `void` |
| `nv_secure_free_TYPE(&vec)` | Zero and free memory | `void` |

### Stack Operations

| Function | Description | Returns |
|----------|-------------|---------|
| `ns_init_TYPE(&stack)` | Initialize empty stack | `void` |
| `ns_push_TYPE(&stack, value)` | Push element | `int` (error code) |
| `ns_pop_TYPE(&stack, &out)` | Pop element | `int` (error code) |
| `ns_peek_TYPE(&stack, &out)` | View top element | `int` (error code) |
| `ns_size_TYPE(&stack)` | Get number of elements | `size_t` |
| `ns_empty_TYPE(&stack)` | Check if empty | `int` (1 if empty) |
| `ns_free_TYPE(&stack)` | Free memory | `void` |

### List Operations

| Function | Description | Returns |
|----------|-------------|---------|
| `nl_init_TYPE(&list)` | Initialize empty list | `void` |
| `nl_push_front_TYPE(&list, value)` | Add to front | `int` (error code) |
| `nl_push_back_TYPE(&list, value)` | Add to back | `int` (error code) |
| `nl_pop_front_TYPE(&list, &out)` | Remove from front | `int` (error code) |
| `nl_size_TYPE(&list)` | Get number of elements | `size_t` |
| `nl_empty_TYPE(&list)` | Check if empty | `int` (1 if empty) |
| `nl_free_TYPE(&list)` | Free memory | `void` |

### Map Operations

| Function | Description | Returns |
|----------|-------------|---------|
| `nm_init(&map)` | Initialize empty map | `void` |
| `nm_init_with_capacity(&map, size)` | Initialize with capacity | `int` (error code) |
| `nm_set(&map, key, value)` | Set key-value pair | `int` (error code) |
| `nm_get(&map, key)` | Get value for key | `void*` or `NULL` |
| `nm_has(&map, key)` | Check if key exists | `int` (1 if exists) |
| `nm_remove(&map, key)` | Remove key-value pair | `int` (error code) |
| `nm_size(&map)` | Get number of entries | `size_t` |
| `nm_empty(&map)` | Check if empty | `int` (1 if empty) |
| `nm_clear(&map)` | Remove all entries | `void` |
| `nm_free(&map)` | Free memory | `void` |
| `nm_secure_free(&map)` | Zero keys and free | `void` |

### Pre-defined Types

```c
// Vectors
IntVector, FloatVector, DoubleVector, CharVector

// Stacks
IntStack, FloatStack, DoubleStack, CharStack

// Lists
IntList, FloatList, DoubleList, CharList

// Define your own
NANODS_DEFINE_VECTOR(YourType)
NANODS_DEFINE_STACK(YourType)
NANODS_DEFINE_LIST(YourType)
```

---

## 📈 Performance

Benchmarked on Linux with GCC -O3 optimization:

| Operation | Performance | Complexity |
|-----------|-------------|------------|
| Vector Push | ~340M ops/sec | O(1) amortized |
| Stack Push/Pop | ~920M ops/sec | O(1) |
| List Push | ~53M ops/sec | O(1) |
| Map Set/Get | ~500K ops/sec | O(1) average |

**Growth Strategy:**
- Vector:   Exponential doubling (2x) with overflow protection
- Map:  Separate chaining with FNV-1a hashing
- Memory:  Contiguous for vectors/stacks (cache-friendly)

---

## 🧪 Testing

Run the comprehensive test suite:

```bash
# Clone and test
git clone https://github.com/Nowazish-Nur-Kayef/NanoDS.git
cd NanoDS
make run
```

**Memory leak verification:**
```bash
make valgrind
```

Expected output:
```
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

**CI/CD:**
- Automated testing on every commit
- Multi-platform:  Ubuntu, macOS, Windows
- Multiple compilers: GCC 9-12, Clang 12-14
- Memory leak detection with Valgrind
- Performance regression checks

---

## 🎯 Use Cases

Perfect for: 

- 🎮 **Game Engines** - Entity lists, component systems, configuration
- 🔧 **Embedded Systems** - Memory-constrained environments with custom allocators
- 🛠️ **System Programming** - Safe data management in low-level code
- 📊 **Data Processing** - Dynamic collections without C++ overhead
- 🔐 **Security Tools** - Secure free for sensitive data handling
- 🚀 **High-Performance** - Cache-friendly contiguous memory layout

---

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

- 🐛 **Report bugs** - [Open an issue](https://github.com/Nowazish-Nur-Kayef/NanoDS/issues)
- 💡 **Suggest features** - Doubly linked lists, queues, trees, etc.
- 📖 **Improve docs** - Fix typos, add examples
- ⭐ **Star the repo** - Help others discover it

### Development

```bash
# Fork and clone
git clone https://github.com/YOUR_USERNAME/NanoDS.git
cd NanoDS

# Make changes
# ...  edit nanods.h ... 

# Test
make clean && make run
make valgrind

# Submit PR
git add . 
git commit -m "Add feature X"
git push origin main
```

---

## 📜 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

You may also choose to use this library under the **Public Domain** (Unlicense) for maximum freedom.

---

## 🙏 Acknowledgments

- Inspired by [stb libraries](https://github.com/nothings/stb) by Sean Barrett
- FNV-1a hash algorithm by Glenn Fowler, Landon Curt Noll, and Kiem-Phong Vo
- Thanks to the C community for feedback and support

---

## 📞 Contact

**Author:** [Nowazish-Nur-Kayef](https://github.com/Nowazish-Nur-Kayef)

- **Issues**: [GitHub Issues](https://github.com/Nowazish-Nur-Kayef/NanoDS/issues)
- **Discussions**: [GitHub Discussions](https://github.com/Nowazish-Nur-Kayef/NanoDS/discussions)

---

## 📚 Documentation

- [API Reference](https://github.com/Nowazish-Nur-Kayef/NanoDS#-api-reference)
- [CHANGELOG](CHANGELOG.md)

---

## ⭐ Show Your Support

If you find **NanoDS** useful, please consider: 

- ⭐ **Starring** the repository
- 🐦 **Sharing** with the C community
- 💬 **Providing feedback** on what works and what doesn't

---

<p align="center">
  <b>Made with ❤️ for the C community</b><br>
  <i>Production-ready • Memory-safe • Zero dependencies</i>
</p>

<p align="center">
  <a href="https://github.com/Nowazish-Nur-Kayef/NanoDS/releases">Download</a> •
  <a href="https://github.com/Nowazish-Nur-Kayef/NanoDS/issues">Report Bug</a> •
  <a href="https://github.com/Nowazish-Nur-Kayef/NanoDS/discussions">Request Feature</a>
</p>

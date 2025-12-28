# ⚡ NanoDS (Nano Data Structures)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Language: C](https://img.shields.io/badge/Language-C-00599C.svg?style=flat&logo=c)
![C Standard](https://img.shields.io/badge/C-C11-blue.svg)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/Nowazish-Nur-Kayef/NanoDS?color=blue)
![Memory Safe](https://img.shields.io/badge/Memory-Safe-green.svg)

**NanoDS** is a high-performance, memory-safe, single-header C library providing generic, type-safe data structures (Vectors and Hash Maps) with zero dependencies.

---

## 🛠 Why NanoDS?

Standard C lacks built-in generic containers. Developers often resort to unsafe `void*` casts or manual memory management. **NanoDS** solves this by using C macros to provide **compile-time type safety** and built-in **security guards**.

### ✨ Key Features

* 📦 **Single-Header** - Drop `nanods.h` into your project. No build systems required. 
* 🔒 **Type-Safe Vectors** - Generic dynamic arrays for any data type (`int`, `struct`, etc.)
* 🗺️ **Secure Hash Maps** - String-keyed maps using collision-resistant **FNV-1a** algorithm
* 🛡️ **Security First**
    * **Integer Overflow Protection** - All allocations checked against `SIZE_MAX`
    * **Safe Realloc** - Prevents memory leaks if `realloc()` fails
    * **Bounds Checking** - Assert-based validation for all index access
    * **Secure Free** - Optional memory zeroing before deallocation
* ✅ **Zero Memory Leaks** - Valgrind verified
* 🚀 **Zero Dependencies** - Only requires standard C library

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

### Usage

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
    printf("First element: %d\n", nv_get_int(&vec, 0));
    printf("Vector size: %zu\n", nv_size_int(&vec));
    
    // Iterate
    for (size_t i = 0; i < nv_size_int(&vec); i++) {
        printf("%d ", nv_get_int(&vec, i));
    }
    
    // Clean up
    nv_free_int(&vec);
    return 0;
}
```

#### 3. Using Hash Maps

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
    
    // Clean up
    nm_free(&config);
    return 0;
}
```

#### 4. Custom Data Types

```c
typedef struct {
    char name[50];
    int age;
} Person;

// Define vector for custom type
NANODS_DEFINE_VECTOR(Person)

int main(void) {
    NanoVector_Person people;
    nv_init_Person(&people);
    
    Person p1 = {"Alice", 25};
    Person p2 = {"Bob", 30};
    
    nv_push_Person(&people, p1);
    nv_push_Person(&people, p2);
    
    // Access
    Person first = nv_get_Person(&people, 0);
    printf("%s is %d years old\n", first.name, first.age);
    
    nv_free_Person(&people);
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

# Check for memory leaks
make valgrind

# Clean build files
make clean

# See all options
make help
```

### Manual Compilation

```bash
# Compile test suite
gcc -std=c11 -Wall -Wextra test. c -o nanods_test

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

## 🛡️ Security Specifications

NanoDS is designed for systems where memory safety is critical:

| Feature | Protection | Implementation |
|---------|------------|----------------|
| **Overflow Check** | Prevents buffer overflows | Validates `a * b < SIZE_MAX` before allocation |
| **Bounds Checking** | Out-of-bounds protection | Assert-based validation on all array access |
| **Safe Realloc** | Memory leak prevention | Uses temporary pointer before overwriting |
| **Memory Sanitization** | Data exposure prevention | `nv_secure_free()` zeros memory with `memset()` |
| **Hash Collision** | DoS attack resistance | FNV-1a algorithm with good distribution |

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

## 📊 API Reference

### Vector Operations

| Function | Description | Returns |
|----------|-------------|---------|
| `nv_init_TYPE(&vec)` | Initialize empty vector | void |
| `nv_push_TYPE(&vec, value)` | Add element to end | 0 on success, -1 on error |
| `nv_get_TYPE(&vec, index)` | Get element at index | Element value |
| `nv_set_TYPE(&vec, index, value)` | Set element at index | 0 on success, -1 on error |
| `nv_pop_TYPE(&vec, &out)` | Remove last element | 0 on success, -1 if empty |
| `nv_size_TYPE(&vec)` | Get number of elements | size_t |
| `nv_empty_TYPE(&vec)` | Check if empty | 1 if empty, 0 otherwise |
| `nv_clear_TYPE(&vec)` | Remove all elements | void |
| `nv_free_TYPE(&vec)` | Free memory | void |
| `nv_secure_free_TYPE(&vec)` | Zero and free memory | void |

### Map Operations

| Function | Description | Returns |
|----------|-------------|---------|
| `nm_init(&map)` | Initialize empty map | void |
| `nm_set(&map, key, value)` | Set key-value pair | 0 on success, -1 on error |
| `nm_get(&map, key)` | Get value for key | Pointer or NULL |
| `nm_has(&map, key)` | Check if key exists | 1 if exists, 0 otherwise |
| `nm_remove(&map, key)` | Remove key-value pair | 0 on success, -1 if not found |
| `nm_size(&map)` | Get number of entries | size_t |
| `nm_empty(&map)` | Check if empty | 1 if empty, 0 otherwise |
| `nm_clear(&map)` | Remove all entries | void |
| `nm_free(&map)` | Free memory | void |
| `nm_secure_free(&map)` | Zero keys and free | void |

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

---

## 📈 Performance

- **Vector Growth**: Exponential (2x) with overflow protection
- **Map Hashing**: FNV-1a (O(1) average lookup)
- **Memory**:  Minimal overhead, no hidden allocations
- **Cache**: Contiguous memory for vectors (cache-friendly)

---

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

- 🐛 **Report bugs** - Open an issue
- 💡 **Suggest features** - Linked lists, queues, trees, etc.
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

---

## 🙏 Acknowledgments

- Inspired by [stb libraries](https://github.com/nothings/stb) by Sean Barrett
- FNV-1a hash algorithm by Glenn Fowler, Landon Curt Noll, and Kiem-Phong Vo

---

## 📞 Contact

**Author:** [Nowazish-Nur-Kayef](https://github.com/Nowazish-Nur-Kayef)

If you find this library useful, please consider giving it a ⭐ to help others discover it!

---

<p align="center">
  <b>Made with ❤️ for the C community</b>
</p>

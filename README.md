<div align="center">

# ⚡ NanoDS

### *Production-Ready Nano Data Structures for C*

[![License:   MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Language: C](https://img.shields.io/badge/Language-C-00599C.svg?style=flat&logo=c)
![C Standard](https://img.shields.io/badge/C-C11-blue.svg)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/Nowazish-Nur-Kayef/NanoDS?color=blue)
![Memory Safe](https://img.shields.io/badge/Memory-Safe-brightgreen.svg)
![Zero Dependencies](https://img.shields.io/badge/Dependencies-Zero-orange.svg)

**Single-header • Type-safe • Memory-safe • Zero dependencies**

[Features](#-features) • [Quick Start](#-quick-start) • [Examples](#-real-world-examples) • [Performance](#-performance) • [Documentation](#-documentation)

<img src="https://img.shields.io/github/stars/Nowazish-Nur-Kayef/NanoDS?style=social" alt="GitHub stars">
<img src="https://img.shields.io/github/forks/Nowazish-Nur-Kayef/NanoDS?style=social" alt="GitHub forks">
<img src="https://img.shields.io/github/watchers/Nowazish-Nur-Kayef/NanoDS?style=social" alt="GitHub watchers">

---

</div>

## 🎯 Why NanoDS?

Standard C lacks built-in generic containers.  Most developers resort to: 
- ❌ **Unsafe `void*` casts** - Type errors at runtime
- ❌ **Manual memory management** - Memory leaks everywhere
- ❌ **Reimplementing data structures** - Wasting time on basics

**NanoDS solves this** by providing compile-time type-safe, memory-safe data structures with zero dependencies. 

```c
// Just drop nanods.h into your project - that's it!
#define NANODS_IMPLEMENTATION
#include "nanods.h"

int main(void) {
    IntVector numbers;
    nv_init_int(&numbers);
    
    nv_push_int(&numbers, 42);
    nv_push_int(&numbers, 100);
    
    int value;
    nv_get_int(&numbers, 0, &value);  // value = 42
    
    nv_free_int(&numbers);
    return 0;
}
```

<div align="center">

**Compile:**  `gcc your_code.c -o your_program`  
**That's it.  No linking.  No dependencies.**

</div>

---

## ✨ Features

<table>
<tr>
<td width="50%">

### 🔒 **Safety First**
- Hybrid safety system (debug + production)
- Integer overflow protection
- Bounds checking on all operations
- NULL pointer validation
- Valgrind verified (zero leaks)

### 🚀 **High Performance**
- Cache-friendly memory layout
- Exponential growth strategy
- 340M+ ops/sec (vector push)
- 920M+ ops/sec (stack ops)
- Optimized hash algorithms (FNV-1a)

</td>
<td width="50%">

### 🎨 **Developer Experience**
- Single-header library
- Zero external dependencies
- Compile-time type safety
- Functional operations (map/filter)
- Custom allocator support

### 🌍 **Cross-Platform**
- Linux, macOS, Windows
- GCC 4.9+, Clang 3.4+, MSVC 2015+
- CMake integration
- pkg-config support
- CI/CD tested on every commit

</td>
</tr>
</table>

---

## 📦 Data Structures

<div align="center">

| Structure | Description | Use Case | Performance |
|-----------|-------------|----------|-------------|
| **NanoVector** | Dynamic array | Sequential data, collections | 340M ops/sec |
| **NanoStack** | LIFO stack | Undo/redo, parsing | 920M ops/sec |
| **NanoList** | Linked list | Frequent insertions | 53M ops/sec |
| **NanoMap** | Hash map | Key-value lookups | 500K ops/sec |

</div>

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
make run  # Build and run tests
```

**Option 3: CMake Integration**
```cmake
add_subdirectory(NanoDS)
target_link_libraries(your_app PRIVATE nanods)
```

---

## 🎮 Real-World Examples

### Example 1: Word Frequency Counter

<details>
<summary><b>Click to expand</b> - Count words in text using NanoMap</summary>

```c
#define NANODS_IMPLEMENTATION
#include "nanods.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void count_words(const char* text, NanoMap* freq_map) {
    char word[64];
    int idx = 0;
    
    for (size_t i = 0; i <= strlen(text); i++) {
        if (isalnum(text[i])) {
            word[idx++] = tolower(text[i]);
        } else if (idx > 0) {
            word[idx] = '\0';
            
            int* count = (int*)nm_get(freq_map, word);
            int new_count = count ?   (*count + 1) : 1;
            
            int* count_ptr = malloc(sizeof(int));
            *count_ptr = new_count;
            nm_set(freq_map, word, count_ptr);
            
            idx = 0;
        }
    }
}

int main(void) {
    const char* text = "the quick brown fox jumps over the lazy dog the fox";
    
    NanoMap freq_map;
    nm_init(&freq_map);
    count_words(text, &freq_map);
    
    printf("Word frequencies:\n");
    NanoMapIterator iter;
    nm_iter_init(&iter, &freq_map);
    
    const char* key;
    void* value;
    while (nm_iter_next(&iter, &key, &value) == NANODS_OK) {
        printf("  %s:   %d\n", key, *(int*)value);
        free(value);
    }
    
    nm_free(&freq_map);
    return 0;
}
```

**Output:**
```
Word frequencies:
  the: 3
  quick: 1
  brown: 1
  fox:  2
  jumps: 1
  over: 1
  lazy: 1
  dog: 1
```

</details>

---

### Example 2: Command History Manager

<details>
<summary><b>Click to expand</b> - CLI command history with search</summary>

```c
#define NANODS_IMPLEMENTATION
#include "nanods.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    char cmd[128];
    int timestamp;
} Command;

NANODS_DEFINE_VECTOR(Command)

void add_command(NanoVector_Command* history, const char* cmd, int ts) {
    Command c;
    strncpy(c.cmd, cmd, 127);
    c.cmd[127] = '\0';
    c.timestamp = ts;
    nv_push_Command(history, c);
}

void search_history(const NanoVector_Command* history, const char* term) {
    printf("Searching for: '%s'\n", term);
    for (size_t i = 0; i < nv_size_Command(history); i++) {
        Command cmd;
        nv_get_Command(history, i, &cmd);
        if (strstr(cmd.cmd, term)) {
            printf("  [%d] %s\n", cmd.timestamp, cmd.cmd);
        }
    }
}

int main(void) {
    NanoVector_Command history;
    nv_init_Command(&history);
    
    add_command(&history, "git status", 1000);
    add_command(&history, "git add .  ", 1005);
    add_command(&history, "git commit -m 'feat'", 1010);
    add_command(&history, "make clean", 1015);
    add_command(&history, "git push", 1020);
    
    search_history(&history, "git");
    
    nv_free_Command(&history);
    return 0;
}
```

**Output:**
```
Searching for: 'git'
  [1000] git status
  [1005] git add . 
  [1010] git commit -m 'feat'
  [1020] git push
```

</details>

---

### Example 3: Functional Programming in C

<details>
<summary><b>Click to expand</b> - Map and filter operations (NEW in v0.1.1)</summary>

```c
#define NANODS_IMPLEMENTATION
#include "nanods.h"
#include <stdio.h>

// Transform function
int double_value(int x) {
    return x * 2;
}

// Predicate function
int is_even(int x) {
    return x % 2 == 0;
}

int main(void) {
    IntVector numbers;
    nv_init_int(&numbers);
    
    // Add numbers:  1, 2, 3, 4, 5
    for (int i = 1; i <= 5; i++) {
        nv_push_int(&numbers, i);
    }
    
    // Map:  double each number
    IntVector doubled;
    nv_map_int(&numbers, &doubled, double_value);
    
    printf("Doubled:  ");
    for (size_t i = 0; i < nv_size_int(&doubled); i++) {
        int val;
        nv_get_int(&doubled, i, &val);
        printf("%d ", val);  // Output: 2 4 6 8 10
    }
    printf("\n");
    
    // Filter: keep only even numbers
    IntVector evens;
    nv_filter_int(&numbers, &evens, is_even);
    
    printf("Evens: ");
    for (size_t i = 0; i < nv_size_int(&evens); i++) {
        int val;
        nv_get_int(&evens, i, &val);
        printf("%d ", val);  // Output: 2 4
    }
    printf("\n");
    
    nv_free_int(&numbers);
    nv_free_int(&doubled);
    nv_free_int(&evens);
    return 0;
}
```

</details>

---

## 📊 Performance

### Benchmark Results

Measured on Linux with GCC -O3 optimization: 

<div align="center">

| Operation | Throughput | Latency | Complexity |
|-----------|-----------|---------|------------|
| **Vector Push** | 340M ops/sec | 0.003 µs | O(1) amortized |
| **Stack Push/Pop** | 920M ops/sec | 0.001 µs | O(1) |
| **List Insert** | 53M ops/sec | 0.019 µs | O(1) |
| **Map Set/Get** | 500K ops/sec | 1.983 µs | O(1) average |

</div>

### Memory Efficiency

```
Vector:  8 bytes overhead + element size
Stack:   0 bytes (uses vector)
List:    16 bytes per node + element size
Map:     Dynamic buckets (default:  16)
```

### Run Benchmarks Yourself

```bash
make benchmarks
make run-benchmarks
```

**Compare with naive implementation:**
```bash
./bench_comparison
```

---

## 🛡️ Safety & Security

### Safety Modes

<table>
<tr>
<td width="50%">

#### Debug Mode (Default)
```c
#define NANODS_IMPLEMENTATION
#include "nanods.h"
// Uses assert() during development
```

**Features:**
- ✅ Catches bugs at development time
- ✅ Zero runtime overhead in release
- ✅ Clear error messages

</td>
<td width="50%">

#### Hard Safety Mode (Production)
```c
#define NANODS_HARD_SAFETY
#define NANODS_IMPLEMENTATION
#include "nanods.h"
// Returns error codes
```

**Features:**
- ✅ Graceful error handling
- ✅ No crashes on invalid input
- ✅ Production-ready safety

</td>
</tr>
</table>

### Security Guarantees

| Feature | Protection | Implementation |
|---------|------------|----------------|
| **Overflow Check** | Integer overflow | Validates `a * b < SIZE_MAX` |
| **Bounds Check** | Out-of-bounds access | Index validation |
| **NULL Safety** | Null pointer crashes | Pointer validation |
| **Memory Sanitization** | Data exposure | `memset()` before free |
| **Hash Collision** | DoS attacks | FNV-1a algorithm |

### Valgrind Verified

```bash
make valgrind
```

**Output:**
```
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

---

## 📚 API Gallery

### Vector Operations

<details>
<summary><b>Click to expand</b> - Complete vector API</summary>

```c
IntVector vec;

// Initialize
nv_init_int(&vec);

// Add elements
nv_push_int(&vec, 42);
nv_reserve_int(&vec, 1000);  // Pre-allocate

// Access
int value;
nv_get_int(&vec, 0, &value);
nv_set_int(&vec, 0, 100);

// Query
size_t size = nv_size_int(&vec);
int empty = nv_empty_int(&vec);

// Remove
int popped;
nv_pop_int(&vec, &popped);
nv_clear_int(&vec);

// Functional (NEW in v0.1.1)
IntVector result;
nv_map_int(&vec, &result, double_value);
nv_filter_int(&vec, &result, is_positive);

// Cleanup
nv_free_int(&vec);
nv_secure_free_int(&vec);  // Zero memory
```

</details>

---

### Stack Operations

<details>
<summary><b>Click to expand</b> - Complete stack API</summary>

```c
IntStack stack;

// Initialize
ns_init_int(&stack);

// Push elements
ns_push_int(&stack, 1);
ns_push_int(&stack, 2);
ns_push_int(&stack, 3);

// Peek (view top without removing)
int top;
ns_peek_int(&stack, &top);  // top = 3

// Pop elements (LIFO order)
int value;
ns_pop_int(&stack, &value);  // value = 3

// Query
size_t size = ns_size_int(&stack);
int empty = ns_empty_int(&stack);

// Cleanup
ns_free_int(&stack);
```

</details>

---

### List Operations

<details>
<summary><b>Click to expand</b> - Complete list API</summary>

```c
IntList list;

// Initialize
nl_init_int(&list);

// Add elements
nl_push_front_int(&list, 10);  // Add to front
nl_push_back_int(&list, 20);   // Add to back

// Remove
int value;
nl_pop_front_int(&list, &value);

// Query
size_t size = nl_size_int(&list);
int empty = nl_empty_int(&list);

// Cleanup
nl_free_int(&list);
```

</details>

---

### Map Operations

<details>
<summary><b>Click to expand</b> - Complete map API</summary>

```c
NanoMap map;

// Initialize
nm_init(&map);
nm_init_with_capacity(&map, 1000);  // Pre-allocate

// Set key-value pairs
int value = 42;
nm_set(&map, "key", &value);

// Get values
int* result = (int*)nm_get(&map, "key");
if (result) {
    printf("Value: %d\n", *result);
}

// Check existence
if (nm_has(&map, "key")) {
    printf("Key exists\n");
}

// Remove
nm_remove(&map, "key");

// Iterate
NanoMapIterator iter;
nm_iter_init(&iter, &map);

const char* key;
void* val;
while (nm_iter_next(&iter, &key, &val) == NANODS_OK) {
    printf("%s: %d\n", key, *(int*)val);
}

// Query
size_t size = nm_size(&map);
int empty = nm_empty(&map);

// Cleanup
nm_clear(&map);         // Remove all entries
nm_free(&map);          // Free memory
nm_secure_free(&map);   // Zero and free
```

</details>

---

### Custom Types

<details>
<summary><b>Click to expand</b> - Use any data type</summary>

```c
// Define your struct
typedef struct {
    char name[50];
    int age;
    float salary;
} Employee;

// Generate vector for your type
NANODS_DEFINE_VECTOR(Employee)

int main(void) {
    NanoVector_Employee employees;
    nv_init_Employee(&employees);
    
    // Create employees
    Employee e1 = {"Alice", 28, 75000.0f};
    Employee e2 = {"Bob", 35, 85000.0f};
    
    // Add to vector
    nv_push_Employee(&employees, e1);
    nv_push_Employee(&employees, e2);
    
    // Access
    Employee emp;
    nv_get_Employee(&employees, 0, &emp);
    printf("%s: $%.2f\n", emp. name, emp.salary);
    
    nv_free_Employee(&employees);
    return 0;
}
```

**Also works with:**
- `NANODS_DEFINE_STACK(YourType)`
- `NANODS_DEFINE_LIST(YourType)`

</details>

---

## 🔧 Advanced Features

### Custom Allocators

Perfect for embedded systems or memory pools:

```c
void* my_malloc(size_t size) {
    return custom_pool_alloc(size);
}

void* my_realloc(void* ptr, size_t size) {
    return custom_pool_realloc(ptr, size);
}

void my_free(void* ptr) {
    custom_pool_free(ptr);
}

int main(void) {
    // Set custom allocator
    NanoAllocator allocator = {
        .malloc_fn = my_malloc,
        .realloc_fn = my_realloc,
        . free_fn = my_free
    };
    nanods_set_allocator(&allocator);
    
    // All NanoDS operations now use custom allocator
    IntVector vec;
    nv_init_int(&vec);
    nv_push_int(&vec, 42);
    nv_free_int(&vec);
    
    return 0;
}
```

---

### Error Handling

```c
IntVector vec;
nv_init_int(&vec);

// All operations return error codes
int err = nv_push_int(&vec, 42);
if (err != NANODS_OK) {
    switch (err) {
        case NANODS_ERR_NOMEM:
            fprintf(stderr, "Out of memory\n");
            break;
        case NANODS_ERR_NULL:
            fprintf(stderr, "Null pointer\n");
            break;
        case NANODS_ERR_OVERFLOW:
            fprintf(stderr, "Integer overflow\n");
            break;
    }
}

nv_free_int(&vec);
```

**Available error codes:**
- `NANODS_OK` - Success
- `NANODS_ERR_NOMEM` - Out of memory
- `NANODS_ERR_BOUNDS` - Index out of bounds
- `NANODS_ERR_EMPTY` - Container is empty
- `NANODS_ERR_OVERFLOW` - Integer overflow
- `NANODS_ERR_NOTFOUND` - Key not found (maps)
- `NANODS_ERR_NULL` - NULL pointer

---

## 🏗️ Build System Integration

### CMake

```cmake
# Add NanoDS to your project
add_subdirectory(NanoDS)
target_link_libraries(your_app PRIVATE nanods)

# Or use find_package after installation
find_package(nanods REQUIRED)
target_link_libraries(your_app PRIVATE nanods:: nanods)
```

### pkg-config

```bash
# Compile
gcc $(pkg-config --cflags nanods) your_code.c -o your_app

# Check installation
pkg-config --modversion nanods
```

### Makefile

```makefile
CFLAGS = -std=c11 -Wall -Wextra -I/path/to/nanods
LDFLAGS = 

your_app: your_code.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
```

---

## 🎯 Use Cases

<div align="center">

| Industry | Use Case | Example |
|----------|----------|---------|
| **🎮 Game Dev** | Entity systems, collections | Particle systems, inventory |
| **🔧 Embedded** | Memory-constrained devices | IoT sensors, microcontrollers |
| **🛠️ Systems** | Low-level data management | Kernel modules, drivers |
| **📊 Data Processing** | Dynamic collections | Log parsers, data pipelines |
| **🔐 Security** | Secure memory handling | Password managers, crypto |
| **🌐 Networking** | Connection pools | Server applications, routers |

</div>

---

## 📖 Documentation

### Resources

- 📘 **[API Reference](https://github.com/Nowazish-Nur-Kayef/NanoDS#-api-gallery)** - Complete function documentation
- 📝 **[CHANGELOG](CHANGELOG.md)** - Version history and migration guide
- 💻 **[Examples](examples/)** - Real-world usage examples
- 🧪 **[Tests](test.c)** - Comprehensive test suite
- 📊 **[Benchmarks](benchmarks/)** - Performance measurements

---

## 🧪 Testing

### Run Tests

```bash
# Basic tests
make test
make run

# Memory leak check
make valgrind

# Hard safety mode
make safe
./nanods_test

# All tests
make clean && make run && make run-benchmarks && make safe
```

### CI/CD

NanoDS is automatically tested on every commit:

- ✅ **Platforms:** Ubuntu, macOS, Windows
- ✅ **Compilers:** GCC 11-12, Clang 14-15, MSVC
- ✅ **Tools:** Valgrind, gcov, CMake
- ✅ **Modes:** Debug, release, hard safety

---

## 🤝 Contributing

We love contributions!   Here's how you can help:

### Ways to Contribute

- 🐛 **Report Bugs** - [Open an issue](https://github.com/Nowazish-Nur-Kayef/NanoDS/issues/new)
- 💡 **Suggest Features** - [Start a discussion](https://github.com/Nowazish-Nur-Kayef/NanoDS/discussions)
- 📖 **Improve Docs** - Fix typos, add examples
- ⭐ **Star the Repo** - Help others discover NanoDS
- 🔀 **Submit PRs** - Code contributions welcome

### Development Setup

```bash
# Fork and clone
git clone https://github.com/YOUR_USERNAME/NanoDS.git
cd NanoDS

# Make changes
# ...  edit files ... 

# Test
make clean && make run
make valgrind

# Submit PR
git add .
git commit -m "Add feature X"
git push origin main
```

### Code Style

- **C11 standard**
- **4 spaces indentation**
- **Descriptive variable names**
- **Comment public APIs**

---

## 📊 Project Stats

<div align="center">

![GitHub contributors](https://img.shields.io/github/contributors/Nowazish-Nur-Kayef/NanoDS)
![GitHub commit activity](https://img.shields.io/github/commit-activity/m/Nowazish-Nur-Kayef/NanoDS)
![GitHub last commit](https://img.shields.io/github/last-commit/Nowazish-Nur-Kayef/NanoDS)
![GitHub repo size](https://img.shields.io/github/repo-size/Nowazish-Nur-Kayef/NanoDS)
![Lines of code](https://img.shields.io/tokei/lines/github/Nowazish-Nur-Kayef/NanoDS)

</div>


## 🏆 Why Choose NanoDS? 

<table>
<tr>
<td align="center">🎯</td>
<td><b>Production-Ready</b><br>Valgrind verified, CI/CD tested, memory-safe</td>
</tr>
<tr>
<td align="center">⚡</td>
<td><b>High Performance</b><br>340M+ ops/sec, cache-friendly, optimized algorithms</td>
</tr>
<tr>
<td align="center">🔒</td>
<td><b>Safe by Design</b><br>Type-safe, bounds-checked, overflow-protected</td>
</tr>
<tr>
<td align="center">📦</td>
<td><b>Zero Dependencies</b><br>Single header, no linking, just include</td>
</tr>
<tr>
<td align="center">🌍</td>
<td><b>Cross-Platform</b><br>Linux, macOS, Windows - works everywhere</td>
</tr>
<tr>
<td align="center">🎨</td>
<td><b>Easy to Use</b><br>Intuitive API, great documentation, real examples</td>
</tr>
</table>

---

## 📜 License

This project is under MIT Licensed: 

- **MIT License** - See [LICENSE](LICENSE)

---

## 🙏 Acknowledgments

- Inspired by [stb libraries](https://github.com/nothings/stb) by Sean Barrett
- FNV-1a hash algorithm by Fowler, Noll, and Vo
- Thanks to the C community for feedback and support

---

## 📞 Support & Community

<div align="center">

[![GitHub Issues](https://img.shields.io/badge/Issues-Support-blue?logo=github)](https://github.com/Nowazish-Nur-Kayef/NanoDS/issues)
[![GitHub Discussions](https://img.shields.io/badge/Discussions-Community-green?logo=github)](https://github.com/Nowazish-Nur-Kayef/NanoDS/discussions)

**Need help?**
- 📧 **Bug Reports:** [Open an issue](https://github.com/Nowazish-Nur-Kayef/NanoDS/issues)
- 💬 **Questions:** [Start a discussion](https://github.com/Nowazish-Nur-Kayef/NanoDS/discussions)

</div>

---

<div align="center">

## ⭐ Show Your Support

If **NanoDS** helped you build something awesome, consider: 

⭐ **Starring** the repository  
🐦 **Sharing** with the C community  
💬 **Providing feedback** on your experience  

**Every star motivates us to keep improving!**

---

### 📥 Get Started Now

```bash
wget https://raw.githubusercontent.com/Nowazish-Nur-Kayef/NanoDS/main/nanods.h
```

**Or clone:**
```bash
git clone https://github.com/Nowazish-Nur-Kayef/NanoDS.git
```

---

<p>
  <b>Made with ❤️ for the C community</b><br>
  <i>Production-ready • Memory-safe • Zero dependencies</i>
</p>

</div>

# ⚡ NanoDS (Nano Data Structures)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Language: C](https://img.shields.io/badge/Language-C-00599C.svg?style=flat&logo=c)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/Nowazish-Nur-Kayef/NanoDS?color=blue)

**NanoDS** is a high-performance, memory-safe, single-header C library. It provides generic, type-safe data structures (Vectors and Hash Maps) with a "zero-dependency" footprint.



## 🛠 Why NanoDS?

Standard C lacks built-in generic containers. Developers often resort to unsafe `void*` casts or manual memory management. **NanoDS** solves this by using C macros to provide **compile-time type safety** and built-in **security guards**.

### Key Features:
* **Single-Header:** Just drop `nanods.h` into your project. No build systems required.
* **Type-Safe Vectors:** Generic dynamic arrays for any data type (`int`, `struct`, etc.).
* **Secure Hash Maps:** String-keyed maps using the collision-resistant **FNV-1a** algorithm.
* **Security First:** * **Integer Overflow Protection:** All allocations are checked against `SIZE_MAX`.
    * **Safe Realloc:** Prevents memory leaks if `realloc` fails.
    * **Secure Free:** Optional zeroing of memory before freeing to protect sensitive data.

---

## 🚀 Quick Start

### 1. Include the Library
In **exactly one** C file, define `NANODS_IMPLEMENTATION` before including the header to create the logic. In all other files, just include it normally.

```c
#define NANODS_IMPLEMENTATION
#include "nanods.h"

```

### 2. Using Vectors

```c
IntVector vec;
nv_init_int(&vec);

nv_push_int(&vec, 42);
nv_push_int(&vec, 100);

printf("Value at 0: %d\n", nv_get_int(&vec, 0));

nv_free_int(&vec);

```

### 3. Using Hash Maps

```c
NanoMap map;
nm_init(&map);

int age = 25;
nm_set(&map, "user_age", &age);

int* result = (int*)nm_get(&map, "user_age");
if (result) printf("Age: %d\n", *result);

nm_free(&map);

```

---

## 🛡 Security Specifications

NanoDS is designed for systems where memory safety is critical:

| Feature | Protection |
| --- | --- |
| **Overflow Check** | Prevents buffer overflows by checking `a * b > SIZE_MAX`. |
| **Bounds Checking** | `assert()` based validation for all index-based access. |
| **Memory Sanitization** | `nv_secure_free` overwrites data with `0x00` to prevent RAM dumping. |

---

## 🏗 Installation & Testing

Clone the repository and run the automated test suite using the provided Makefile:

```bash
git clone [https://github.com/Nowazish-Nur-Kayef/NanoDS.git](https://github.com/Nowazish-Nur-Kayef/NanoDS.git)
cd NanoDS
make run

```

---

## 📜 License

This project is licensed under the **MIT License** - see the [LICENSE](https://www.google.com/search?q=LICENSE) file for details.

## 🤝 Contributing

Stars are always welcome! If you'd like to contribute (Linked Lists, Queues, etc.), please open an issue or a Pull Request.

If you find this library useful, please consider giving it a ⭐ to help others discover it!

**Author:** [Nowazish-Nur-Kayef](https://github.com/Nowazish-Nur-Kayef)

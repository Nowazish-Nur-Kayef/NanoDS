# NanoDS Design Documentation

## Architecture Overview

NanoDS follows the **STB-style single-header library** pattern, providing production-grade data structures with compile-time type safety and zero runtime dependencies. 

---

## Memory Layout

### Vector Memory Layout

```
┌─────────────────────────────────────┐
│  NanoVector_int                     │
├─────────────────────────────────────┤
│  T* data         (8 bytes)          │  ← Pointer to heap-allocated array
│  size_t size     (8 bytes)          │  ← Current number of elements
│  size_t capacity (8 bytes)          ���  ← Allocated capacity
│  uint8_t flags   (1 byte)           │  ← Flags (secure, etc.)
│  [padding]       (7 bytes)          │  ← Alignment padding
└─────────────────────────────────────┘
Total: 32 bytes (stack)

Heap allocation: 
┌─────────────────────────────────────┐
│  Element[0]                         │
│  Element[1]                         │
│  ...                                │
│  Element[size-1]                    │
│  [unused capacity]                  │
└─────────────────────────────────────┘
Total: capacity * sizeof(T) bytes
```

**Key Points:**
- Metadata is stored on the stack (32 bytes)
- Actual data is heap-allocated
- No hidden headers before data pointer (cache-friendly)
- Growth is exponential (2x) for amortized O(1) push

---

### List Node Layout

```
Singly Linked List Node:
┌─────────────────────────────────────┐
│  T data                             │  ← Element data
│  NanoListNode_T* next (8 bytes)    │  ← Pointer to next node
└─────────────────────────────────────┘
Total: sizeof(T) + 8 bytes per node

Doubly Linked List Node: 
┌─────────────────────────────────────┐
│  T data                             │  ← Element data
│  NanoListNode2_T* prev (8 bytes)   │  ← Pointer to previous node
│  NanoListNode2_T* next (8 bytes)   │  ← Pointer to next node
└─────────────────────────────────────┘
Total: sizeof(T) + 16 bytes per node
```

**Key Points:**
- Each node is individually allocated
- No metadata overhead per-list (only head/tail/size)
- Doubly linked lists enable O(1) bidirectional traversal

---

### Map Memory Layout

```
NanoMap Structure:
┌─────────────────────────────────────┐
│  NanoMapEntry** buckets (8 bytes)  │  ← Array of bucket pointers
│  size_t bucket_count (8 bytes)     │  ← Number of buckets
│  size_t size         (8 bytes)     │  ← Number of entries
│  uint32_t seed       (4 bytes)     │  ← Hash seed (Anti-DoS)
│  uint8_t flags       (1 byte)      │  ← Flags
│  [padding]           (3 bytes)     │  ← Alignment
└─────────────────────────────────────┘
Total:  32 bytes (stack)

Bucket Array:
┌─────────────────────────────────────┐
│  Bucket[0] → Entry → Entry → NULL   │
│  Bucket[1] → NULL                   │
│  Bucket[2] → Entry → NULL           │
│  ...                                │
└─────────────────────────────────────┘

Entry Layout:
┌─────────────────────────────────────┐
│  char* key         (8 bytes)        │  ← Heap-allocated string
│  void* value       (8 bytes)        │  ← User value pointer
│  NanoMapEntry* next (8 bytes)       │  ← Next in chain
└─────────────────────────────────────┘
Total: 24 bytes + strlen(key) + 1
```

**Key Points:**
- Default 16 buckets, grows as needed
- Separate chaining for collision resolution
- FNV-1a hash with randomized seed (防DoS)
- Load factor:  size / bucket_count

---

### Ring Buffer Layout

```
NanoRing_int_64:
┌─────────────────────────────────────┐
│  T data[SIZE]     (SIZE * sizeof(T))│  ← Fixed-size array on stack
│  size_t head      (8 bytes)         │  ← Read position
│  size_t tail      (8 bytes)         │  ← Write position
│  size_t count     (8 bytes)         │  ← Current element count
│  size_t capacity  (8 bytes)         │  ← Fixed capacity (SIZE)
│  uint8_t flags    (1 byte)          │  ← Flags
│  [padding]        (7 bytes)         │  ← Alignment
└─────────────────────────────────────┘
Total: SIZE * sizeof(T) + 40 bytes (stack)
```

**Key Points:**
- **Zero heap allocations** (entirely stack-based)
- Fixed size determined at compile time
- Circular indexing:  `(index + 1) % capacity`
- Perfect for embedded systems and real-time streams

---

## Growth Strategy

### Vector Exponential Growth

**Growth Factor:  2x (doubling)**

```c
new_capacity = old_capacity == 0 ? 8 : old_capacity * 2
```

**Why 2x?**

| Strategy | Pros | Cons | Choice |
|----------|------|------|--------|
| **1.5x (φ)** | Better memory reuse | More reallocations | ❌ |
| **2x** | Fewer reallocations, cache-friendly | More wasted space | ✅ |
| **3x** | Very few reallocations | Excessive waste | ❌ |

**Analysis:**
- **Amortized O(1) push**: Each element is copied at most `log₂(n)` times
- **Memory overhead**: At most 50% wasted capacity
- **Cache performance**: Large contiguous blocks are cache-friendly
- **Industry standard**: Used by C++ STL, Rust Vec, Go slices

**Growth Sequence:**
```
Push #1:   capacity = 8   (initial)
Push #9:   capacity = 16  (2x)
Push #17:  capacity = 32  (2x)
Push #33:  capacity = 64  (2x)
Push #65:  capacity = 128 (2x)
```

**Total copies for n=1000 pushes:**
```
Copies = 8 + 8 + 16 + 32 + 64 + 128 + 256 + 512 = 1024
Average per element = 1024 / 1000 = 1.024 copies
```

**Amortized cost:** O(1) per push

---

### Map Load Factor & Rehashing

**Default bucket count: 16**
**Load factor threshold: 0.75** (typical)

When `size / bucket_count > 0.75`, rehash to `2 * bucket_count`.

**Why 0.75?**
- Below 0.75: Good performance, low collision rate
- Above 0.75: Performance degradation due to collisions

**Collision Resolution:** Separate chaining (linked lists per bucket)

---

## Security Features

### 1. Hash Randomization (Anti-DoS)

**Problem:** Hash-flooding attacks force all keys into one bucket, degrading O(1) to O(n).

**Solution:** Randomized hash seed

```c
uint32_t seed = time(NULL) ^ (uintptr_t)&global_var;
hash = FNV1a(key) ^ seed;
```

**Protection:**
- Attacker cannot predict which keys collide
- Seed changes per program run (or can be set manually)
- Industry standard (used by Python, Ruby, Java)

**Usage:**
```c
nanods_seed_init(0);  // Auto-generate seed at startup
// or
nanods_seed_init(0x12345678);  // Custom seed
```

---

### 2. Secure Memory Wiping

**Problem:** Sensitive data (passwords, keys) may remain in memory after free.

**Solution:** `NANODS_FLAG_SECURE`

```c
IntVector vec;
nv_init_ex_int(&vec, NANODS_FLAG_SECURE);
// ...  use vector ...
nv_free_int(&vec);  // Automatically memset(0) before free
```

**What it does:**
```c
if (flags & NANODS_FLAG_SECURE) {
    memset(data, 0, capacity * sizeof(T));
}
free(data);
```

**Use cases:**
- Password storage
- Cryptographic keys
- Sensitive user data

---

### 3. Integer Overflow Protection

**Problem:** `size * sizeof(T)` can overflow on 32-bit systems or with huge allocations.

**Solution:** Check before allocation

```c
static inline int nanods_check_mul_overflow(size_t a, size_t b, size_t* result) {
    if (a > 0 && b > SIZE_MAX / a) return 1;  // Overflow detected
    *result = a * b;
    return 0;
}
```

**Applied everywhere:**
```c
size_t byte_size;
if (nanods_check_mul_overflow(capacity, sizeof(T), &byte_size))
    return NANODS_ERR_OVERFLOW;
```

---

## Compiler Optimizations

### Branch Prediction Hints

```c
#define NANODS_LIKELY(x)   __builtin_expect(!!(x), 1)
#define NANODS_UNLIKELY(x) __builtin_expect(!!(x), 0)
```

**Usage:**
```c
if (NANODS_UNLIKELY(! ptr)) return NANODS_ERR_NULL;  // Rare error path
if (NANODS_LIKELY(size < capacity)) { ... }         // Common fast path
```

**Effect:**
- CPU can predict branches better
- Reduces pipeline stalls
- ~5-10% performance improvement in tight loops

**Where applied:**
- NULL checks (errors are unlikely)
- Bounds checks (usually in bounds)
- Memory allocation failures (rarely fail)

---

## Error Handling Philosophy

### Dual-Mode Safety

**Debug Mode** (default):
```c
assert(ptr != NULL && "Null pointer");
```
- Fast (zero overhead in release builds)
- Clear error messages during development
- Crashes immediately on bugs

**Hard Safety Mode** (`-DNANODS_HARD_SAFETY`):
```c
if (! ptr) return NANODS_ERR_NULL;
```
- Never crashes
- Returns error codes
- Production-safe

**Usage:**
```c
// Debug:  assert fires if error
nv_push_int(&vec, 42);

// Hard safety: check return value
int err = nv_push_int(&vec, 42);
if (err != NANODS_OK) {
    fprintf(stderr, "Error: %d\n", err);
}
```

---

## Performance Characteristics

### Big-O Complexity Table

| Data Structure | Operation | Best | Average | Worst | Notes |
|----------------|-----------|------|---------|-------|-------|
| **Vector** | `push` | O(1) | O(1) | O(n) | Amortized O(1) |
| | `pop` | O(1) | O(1) | O(1) | |
| | `get/set` | O(1) | O(1) | O(1) | Direct indexing |
| | `reserve` | O(n) | O(n) | O(n) | Reallocation |
| | `map/filter` | O(n) | O(n) | O(n) | Linear scan |
| **Stack** | `push` | O(1) | O(1) | O(n) | Uses vector |
| | `pop` | O(1) | O(1) | O(1) | |
| | `peek` | O(1) | O(1) | O(1) | |
| **List** | `push_front` | O(1) | O(1) | O(1) | |
| | `push_back` | O(1) | O(1) | O(1) | Tail pointer |
| | `pop_front` | O(1) | O(1) | O(1) | |
| | `access[i]` | O(n) | O(n) | O(n) | No indexing |
| **List2** | `push_front/back` | O(1) | O(1) | O(1) | |
| | `pop_front/back` | O(1) | O(1) | O(1) | |
| | `insert_after` | O(1) | O(1) | O(1) | With node ref |
| | `remove_node` | O(1) | O(1) | O(1) | With node ref |
| **Map** | `set` | O(1) | O(1) | O(n) | O(n) if all collide |
| | `get` | O(1) | O(1) | O(n) | |
| | `remove` | O(1) | O(1) | O(n) | |
| | `has` | O(1) | O(1) | O(n) | |
| **Ring** | `write` | O(1) | O(1) | O(1) | Fixed size |
| | `read` | O(1) | O(1) | O(1) | |
| | `peek` | O(1) | O(1) | O(1) | |

**Space Complexity:**

| Structure | Space | Notes |
|-----------|-------|-------|
| Vector | O(n) | 32 bytes + n * sizeof(T) |
| Stack | O(n) | Same as vector |
| List | O(n) | n * (sizeof(T) + 8 bytes) |
| List2 | O(n) | n * (sizeof(T) + 16 bytes) |
| Map | O(n + b) | n entries + b buckets |
| Ring | O(capacity) | Fixed, stack-allocated |

---

## Thread Safety

**NanoDS is NOT thread-safe by default.**

For multi-threaded use: 
1. **Option 1:** External locking
   ```c
   pthread_mutex_t lock;
   pthread_mutex_lock(&lock);
   nv_push_int(&vec, 42);
   pthread_mutex_unlock(&lock);
   ```

2. **Option 2:** Thread-local storage
   ```c
   __thread IntVector my_vec;
   ```

3. **Option 3:** Lock-free (advanced)
   - Use atomic operations
   - Requires careful design

---

## Custom Allocators

**Use case:** Embedded systems, memory pools, debugging

```c
void* my_malloc(size_t size) {
    void* ptr = pool_alloc(size);
    log_allocation(size, ptr);
    return ptr;
}

NanoAllocator allocator = {
    .malloc_fn = my_malloc,
    .realloc_fn = my_realloc,
    .free_fn = my_free
};
nanods_set_allocator(&allocator);
```

**All allocations** now go through custom allocator.

---

## Benchmarking Methodology

**Hardware:**
- CPU: Intel i7-10700K @ 3.8GHz
- RAM: 32GB DDR4 @ 3200MHz
- OS: Linux 5.15, GCC 11.2

**Compiler flags:**
```bash
gcc -std=c11 -O3 -march=native -Wall -Wextra
```

**Measurement:**
- High-resolution timing (`clock_gettime(CLOCK_MONOTONIC)`)
- Warm-up runs to prime CPU cache
- Multiple iterations (1M ops)
- Median of 10 runs

**Metrics:**
- **Throughput:** Operations per second
- **Latency:** Microseconds per operation
- **Memory:** Bytes allocated (via Valgrind Massif)

---

## Future Improvements

### v1.1.0 (Planned)
- [ ] Priority queue (binary heap)
- [ ] Binary search tree
- [ ] More iterator combinators (zip, fold)
- [ ] Concurrent data structures (optional)

### v1.2.0 (Future)
- [ ] Graph data structures
- [ ] String utilities
- [ ] SIMD optimizations

---

## References

- **FNV-1a Hash:** http://www.isthe.com/chongo/tech/comp/fnv/
- **Amortized Analysis:** CLRS, Chapter 17
- **Cache-Friendly Design:** "What Every Programmer Should Know About Memory" (Ulrich Drepper)
- **STB Libraries:** https://github.com/nothings/stb

---

**NanoDS v1.0.0 - Production Standard**  
*Designed for performance, safety, and simplicity.*
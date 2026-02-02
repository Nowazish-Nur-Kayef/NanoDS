#!/bin/bash

echo "========================================"
echo "  NanoDS v1.0.0 Benchmark Suite"
echo "========================================"
echo ""

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    CC=gcc
elif [[ "$OSTYPE" == "darwin"* ]]; then
    CC=clang
else
    CC=gcc
fi

echo "Compiler: $CC"
echo ""

# Build benchmarks
echo "Building benchmarks..."
$CC -std=c11 -Wall -Wextra -O3 -march=native bench_vector.c -o bench_vector
$CC -std=c11 -Wall -Wextra -O3 -march=native bench_map.c -o bench_map
$CC -std=c11 -Wall -Wextra -O3 -march=native bench_comparison.c -o bench_comparison
$CC -std=c11 -Wall -Wextra -O3 -march=native bench_list2.c -o bench_list2
$CC -std=c11 -Wall -Wextra -O3 -march=native bench_ring.c -o bench_ring

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build successful!"
echo ""

# Run benchmarks
echo "========================================"
echo "Running Vector Benchmark..."
echo "========================================"
./bench_vector
echo ""

echo "========================================"
echo "Running Map Benchmark..."
echo "========================================"
./bench_map
echo ""

echo "========================================"
echo "Running Comparison Benchmark..."
echo "========================================"
./bench_comparison
echo ""

echo "========================================"
echo "Running Doubly Linked List Benchmark..."
echo "========================================"
./bench_list2
echo ""

echo "========================================"
echo "Running Ring Buffer Benchmark..."
echo "========================================"
./bench_ring
echo ""

echo "========================================"
echo "Benchmarks complete!"
echo "========================================"

# Cleanup
rm -f bench_vector bench_map bench_comparison bench_list2 bench_ring
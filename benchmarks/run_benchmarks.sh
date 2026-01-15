#!/bin/bash

echo "========================================"
echo "  NanoDS Benchmark Suite"
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

# Generate CSV
echo "Generating results. csv..."
echo "benchmark,iterations,time_ms,ops_per_sec" > results.csv
./bench_vector | grep -A3 "Sequential Push" | tail -1 | awk '{print "vector_push,1000000," $2 "," $4}' >> results.csv

echo ""
echo "========================================"
echo "Benchmarks complete!"
echo "Results saved to results.csv"
echo "========================================"

# Cleanup
rm -f bench_vector bench_map bench_comparison

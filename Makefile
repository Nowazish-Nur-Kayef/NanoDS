# =============================================================================
# NanoDS v1.0.0 Makefile
# =============================================================================

ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    TARGET_EXT = .exe
    RM = del /Q
else
    DETECTED_OS := $(shell uname -s)
    TARGET_EXT =
    RM = rm -f
endif

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
LDFLAGS =

# Targets
TARGET_TEST = nanods_test$(TARGET_EXT)
TARGET_BENCH_VEC = bench_vector$(TARGET_EXT)
TARGET_BENCH_MAP = bench_map$(TARGET_EXT)
TARGET_BENCH_CMP = bench_comparison$(TARGET_EXT)
TARGET_BENCH_LIST2 = bench_list2$(TARGET_EXT)
TARGET_BENCH_RING = bench_ring$(TARGET_EXT)
TARGET_WORD_FREQ = word_frequency$(TARGET_EXT)
TARGET_CMD_HIST = command_history$(TARGET_EXT)
TARGET_RING_EX = ring_buffer_example$(TARGET_EXT)
TARGET_ITER_EX = iterator_example$(TARGET_EXT)

# Source files
SRC_TEST = test.c
SRC_BENCH_VEC = benchmarks/bench_vector.c
SRC_BENCH_MAP = benchmarks/bench_map.c
SRC_BENCH_CMP = benchmarks/bench_comparison.c
SRC_BENCH_LIST2 = benchmarks/bench_list2.c
SRC_BENCH_RING = benchmarks/bench_ring.c
SRC_WORD_FREQ = examples/word_frequency.c
SRC_CMD_HIST = examples/command_history.c
SRC_RING_EX = examples/ring_buffer_example. c
SRC_ITER_EX = examples/iterator_example.c
HEADER = nanods.h

# Build modes
DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG -march=native
SAFE_FLAGS = -DNANODS_HARD_SAFETY

. PHONY: all test examples benchmarks debug release safe clean run bundle help install

# Default target
all: test

# Build test suite
test: $(SRC_TEST) $(HEADER)
	@echo "Building test suite for $(DETECTED_OS)..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_TEST) -o $(TARGET_TEST)
	@echo "✅ Built $(TARGET_TEST)"

# Build examples
examples: word-freq cmd-hist ring-ex iter-ex

word-freq: $(SRC_WORD_FREQ) $(HEADER)
	@echo "Building word frequency example..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_WORD_FREQ) -o $(TARGET_WORD_FREQ)
	@echo "✅ Built $(TARGET_WORD_FREQ)"

cmd-hist: $(SRC_CMD_HIST) $(HEADER)
	@echo "Building command history example..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_CMD_HIST) -o $(TARGET_CMD_HIST)
	@echo "✅ Built $(TARGET_CMD_HIST)"

ring-ex: $(SRC_RING_EX) $(HEADER)
	@echo "Building ring buffer example..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_RING_EX) -o $(TARGET_RING_EX)
	@echo "✅ Built $(TARGET_RING_EX)"

iter-ex: $(SRC_ITER_EX) $(HEADER)
	@echo "Building iterator example..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_ITER_EX) -o $(TARGET_ITER_EX)
	@echo "✅ Built $(TARGET_ITER_EX)"

# Build benchmarks
benchmarks: bench-vector bench-map bench-comparison bench-list2 bench-ring

bench-vector: $(SRC_BENCH_VEC) $(HEADER)
	@echo "Building vector benchmark..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_BENCH_VEC) -o $(TARGET_BENCH_VEC)
	@echo "✅ Built $(TARGET_BENCH_VEC)"

bench-map: $(SRC_BENCH_MAP) $(HEADER)
	@echo "Building map benchmark..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_BENCH_MAP) -o $(TARGET_BENCH_MAP)
	@echo "✅ Built $(TARGET_BENCH_MAP)"

bench-comparison: $(SRC_BENCH_CMP) $(HEADER)
	@echo "Building comparison benchmark..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_BENCH_CMP) -o $(TARGET_BENCH_CMP)
	@echo "✅ Built $(TARGET_BENCH_CMP)"

bench-list2: $(SRC_BENCH_LIST2) $(HEADER)
	@echo "Building doubly linked list benchmark..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_BENCH_LIST2) -o $(TARGET_BENCH_LIST2)
	@echo "✅ Built $(TARGET_BENCH_LIST2)"

bench-ring: $(SRC_BENCH_RING) $(HEADER)
	@echo "Building ring buffer benchmark..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_BENCH_RING) -o $(TARGET_BENCH_RING)
	@echo "✅ Built $(TARGET_BENCH_RING)"

# Build with debug symbols
debug: $(SRC_TEST) $(HEADER)
	@echo "Building debug version..."
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SRC_TEST) -o $(TARGET_TEST)
	@echo "🐛 Built $(TARGET_TEST) (debug mode)"

# Build optimized release
release: $(SRC_TEST) $(HEADER)
	@echo "Building optimized release..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_TEST) -o $(TARGET_TEST)
	@echo "🚀 Built $(TARGET_TEST) (release mode)"

# Build with hard safety
safe: $(SRC_TEST) $(HEADER)
	@echo "Building with hard safety..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SAFE_FLAGS) $(SRC_TEST) -o $(TARGET_TEST)
	@echo "🛡️  Built $(TARGET_TEST) (hard safety mode)"

# Run tests
run: test
	@echo "🧪 Running tests..."
	./$(TARGET_TEST)

# Run examples
run-examples: examples
	@echo "🎮 Running word frequency example..."
	./$(TARGET_WORD_FREQ)
	@echo ""
	@echo "🎮 Running command history example..."
	./$(TARGET_CMD_HIST)
	@echo ""
	@echo "🎮 Running ring buffer example..."
	./$(TARGET_RING_EX)
	@echo ""
	@echo "🎮 Running iterator example..."
	./$(TARGET_ITER_EX)

# Run benchmarks
run-benchmarks:  benchmarks
	@echo "📊 Running benchmarks..."
	./$(TARGET_BENCH_VEC)
	@echo ""
	./$(TARGET_BENCH_MAP)
	@echo ""
	./$(TARGET_BENCH_CMP)
	@echo ""
	./$(TARGET_BENCH_LIST2)
	@echo ""
	./$(TARGET_BENCH_RING)

# Bundle single-file header
bundle: 
	@echo "📦 Creating bundled single-file header..."
	python3 scripts/bundle. py
	@echo "✅ Created nanods_bundled.h"

# Memory leak check with Valgrind (Linux/macOS only)
valgrind:  debug
	@echo "🔍 Running Valgrind..."
ifeq ($(DETECTED_OS),Linux)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET_TEST)
else
	@echo "⚠️  Valgrind only available on Linux"
endif

# Clean build artifacts
clean:
	@echo "🧹 Cleaning..."
	$(RM) $(TARGET_TEST) $(TARGET_BENCH_VEC) $(TARGET_BENCH_MAP) $(TARGET_BENCH_CMP)
	$(RM) $(TARGET_BENCH_LIST2) $(TARGET_BENCH_RING)
	$(RM) $(TARGET_WORD_FREQ) $(TARGET_CMD_HIST) $(TARGET_RING_EX) $(TARGET_ITER_EX)
	$(RM) nanods_bundled.h
	$(RM) *.o *.out core core.* vgcore.* a.out test_bundle test_bundle.c
	@echo "✅ Clean complete"

# Install header to system (Linux/macOS only)
install:
	@echo "📥 Installing NanoDS v1.0.0..."
ifeq ($(DETECTED_OS),Linux)
	sudo mkdir -p /usr/local/include/nanods/src
	sudo mkdir -p /usr/local/share/doc/nanods
	sudo cp nanods.h /usr/local/include/
	sudo cp -r src/*. h /usr/local/include/nanods/src/
	sudo cp -r docs/*. md /usr/local/share/doc/nanods/
	@echo "✅ Installation complete"
else ifeq ($(DETECTED_OS),Darwin)
	sudo mkdir -p /usr/local/include/nanods/src
	sudo mkdir -p /usr/local/share/doc/nanods
	sudo cp nanods.h /usr/local/include/
	sudo cp -r src/*.h /usr/local/include/nanods/src/
	sudo cp -r docs/*.md /usr/local/share/doc/nanods/
	@echo "✅ Installation complete"
else
	@echo "⚠️  Manual installation required on Windows"
endif

# Show help
help:
	@echo "========================================"
	@echo "NanoDS v1.0.0 Build System"
	@echo "========================================"
	@echo ""
	@echo "Targets:"
	@echo "  make                - Build test suite (default)"
	@echo "  make test           - Build test suite"
	@echo "  make examples       - Build all examples"
	@echo "  make benchmarks     - Build all benchmarks"
	@echo "  make debug          - Build with debug symbols"
	@echo "  make release        - Build optimized release"
	@echo "  make safe           - Build with hard safety"
	@echo "  make run            - Build and run tests"
	@echo "  make run-examples   - Build and run examples"
	@echo "  make run-benchmarks - Build and run benchmarks"
	@echo "  make bundle         - Create single-file header"
	@echo "  make valgrind       - Run memory leak check"
	@echo "  make install        - Install to system"
	@echo "  make clean          - Remove build artifacts"
	@echo "  make help           - Show this help"
	@echo ""
	@echo "New in v1.0.0:"
	@echo "  - Doubly linked lists (List2)"
	@echo "  - Ring buffers (fixed-size)"
	@echo "  - Hash randomization (Anti-DoS)"
	@echo "  - Secure memory wiping"
	@echo "  - Universal iterators"
	@echo ""

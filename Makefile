# =============================================================================
# NanoDS Production Makefile
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

TARGET_TEST = nanods_test$(TARGET_EXT)
TARGET_BENCHMARK = nanods_benchmark$(TARGET_EXT)

SRC_TEST = test.c
SRC_BENCHMARK = benchmark.c
HEADER = nanods.h

DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG -march=native
SAFE_FLAGS = -DNANODS_HARD_SAFETY

.  PHONY: all test benchmark debug release safe clean valgrind help

all: test

test: $(SRC_TEST) $(HEADER)
	@echo "Building test suite for $(DETECTED_OS)..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_TEST) -o $(TARGET_TEST)
	@echo "✅ Built $(TARGET_TEST)"

benchmark: $(SRC_BENCHMARK) $(HEADER)
	@echo "Building benchmark for $(DETECTED_OS)..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_BENCHMARK) -o $(TARGET_BENCHMARK)
	@echo "✅ Built $(TARGET_BENCHMARK)"

debug: $(SRC_TEST) $(HEADER)
	@echo "Building debug version..."
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SRC_TEST) -o $(TARGET_TEST)
	@echo "🐛 Built $(TARGET_TEST) (debug mode)"

release: $(SRC_TEST) $(HEADER)
	@echo "Building optimized release..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRC_TEST) -o $(TARGET_TEST)
	@echo "🚀 Built $(TARGET_TEST) (release mode)"

safe: $(SRC_TEST) $(HEADER)
	@echo "Building with hard safety..."
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SAFE_FLAGS) $(SRC_TEST) -o $(TARGET_TEST)
	@echo "🛡️  Built $(TARGET_TEST) (hard safety mode)"

run: test
	@echo "🧪 Running tests..."
	./$(TARGET_TEST)

run-benchmark: benchmark
	@echo "📊 Running benchmark..."
	./$(TARGET_BENCHMARK)

valgrind: debug
	@echo "🔍 Running Valgrind..."
ifeq ($(DETECTED_OS),Linux)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET_TEST)
else
	@echo "⚠️  Valgrind only available on Linux"
endif

clean:
	@echo "🧹 Cleaning..."
	$(RM) $(TARGET_TEST) $(TARGET_BENCHMARK)
	$(RM) *.o *.out core core.* vgcore.* a.out
	@echo "✅ Clean complete"

help:
	@echo "================================"
	@echo "NanoDS Build System v0.1.0"
	@echo "================================"
	@echo ""
	@echo "Targets:"
	@echo "  make              - Build test suite"
	@echo "  make test         - Build test suite"
	@echo "  make benchmark    - Build benchmark"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make release      - Build optimized release"
	@echo "  make safe         - Build with hard safety"
	@echo "  make run          - Build and run tests"
	@echo "  make run-benchmark - Build and run benchmark"
	@echo "  make valgrind     - Run memory leak check"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make help         - Show this help"
	@echo ""

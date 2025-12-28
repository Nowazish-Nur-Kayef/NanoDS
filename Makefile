# Detect OS
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
CFLAGS = -std=c11 -Wall -Wextra
TARGET = nanods_test$(TARGET_EXT)

all: 
	@echo "Building for $(DETECTED_OS)..."
	$(CC) $(CFLAGS) -O3 test.c -o $(TARGET)

clean:
	$(RM) $(TARGET)

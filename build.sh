#!/bin/bash
# Build script for Unix/Linux/macOS environments

set -e  # Exit on error

echo ""
echo "Kernel Panic Logger - Unix Build Script"
echo "========================================"
echo ""

# Create directories
mkdir -p build
mkdir -p bin

# Compile object files
echo "Compiling log_buffer.c..."
gcc -Wall -Wextra -std=c99 -I./include -c src/log_buffer.c -o build/log_buffer.o

echo "Compiling crash_context.c..."
gcc -Wall -Wextra -std=c99 -I./include -c src/crash_context.c -o build/crash_context.o

echo "Compiling example.c..."
gcc -Wall -Wextra -std=c99 -I./include -c src/example.c -o build/example.o

# Link executable
echo "Linking..."
gcc build/log_buffer.o build/crash_context.o build/example.o -o bin/kernel_panic_logger

echo ""
echo "✓ Build successful!"
echo "Output: bin/kernel_panic_logger"
echo ""

# Run the demo
echo "Running demo..."
echo ""
./bin/kernel_panic_logger
echo ""

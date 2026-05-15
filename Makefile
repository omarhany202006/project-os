CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -I./include
LDFLAGS = 

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Source files
SOURCES = $(SRC_DIR)/log_buffer.c $(SRC_DIR)/crash_context.c $(SRC_DIR)/example.c
OBJECTS = $(BUILD_DIR)/log_buffer.o $(BUILD_DIR)/crash_context.o $(BUILD_DIR)/example.o
EXECUTABLE = $(BIN_DIR)/kernel_panic_logger

# Default target
all: $(EXECUTABLE)

# Create build directories
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(EXECUTABLE): $(OBJECTS) $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)
	@echo "✓ Build successful: $(EXECUTABLE)"

# Run the program
run: $(EXECUTABLE)
	@echo "Running kernel panic logger demo..."
	@$(EXECUTABLE)

# Clean build artifacts
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "✓ Build cleaned"

# Rebuild everything
rebuild: clean all

# Display usage
help:
	@echo "Kernel Panic Logger - Makefile Targets:"
	@echo "  make all      - Build the project (default)"
	@echo "  make run      - Build and run the demo"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make rebuild  - Clean and rebuild"
	@echo "  make help     - Show this help message"

.PHONY: all run clean rebuild help

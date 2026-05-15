# Kernel Panic Logger

A robust kernel panic logging and crash handling system with circular log buffers and comprehensive crash context capture.

## Features

### Log Buffer System
- **Circular Ring Buffer**: 256 KB buffer with automatic wrap-around
- **Structured Log Entries**: Timestamp, CPU ID, log level, and message
- **Multiple Log Levels**: DEBUG, INFO, WARN, ERROR, PANIC
- **Overflow Tracking**: Monitors when buffer wraps around
- **Efficient Storage**: Fixed-size entries for deterministic behavior

### Crash Context Information
- **CPU Register State**: Captures all x86-64 general and control registers
- **Stack Trace**: Records up to 32 stack frames with symbol information
- **Kernel Stack Information**: Tracks stack bounds and size
- **Exception Details**: Exception number and human-readable names
- **Process Information**: PID, TID, and process name at crash
- **System State**: Uptime, IRQ level, and interrupt context

### Exception Handling
Supports 21 x86-64 exception types:
- Division by Zero
- Debug
- Non-Maskable Interrupt
- Breakpoint
- Overflow
- Bound Range Exceeded
- Invalid Opcode
- Device Not Available
- Double Fault
- Coprocessor Segment Overrun
- Invalid TSS
- Segment Not Present
- Stack-Segment Fault
- General Protection Fault
- Page Fault
- x87 Floating-Point Exception
- Alignment Check
- Machine Check
- SIMD Floating-Point Exception
- Virtualization Exception

## Project Structure

```
project os/
|-- include/
|   |-- log_buffer.h        # Log buffer API
|   `-- crash_context.h     # Crash context API
|-- src/
|   |-- log_buffer.c        # Log buffer implementation
|   |-- crash_context.c     # Crash context implementation
|   `-- example.c           # Demonstration program
|-- Makefile                # Build configuration
|-- README.md               # This file
`-- docs/
    `-- API_REFERENCE.md    # Detailed API documentation
```

## Building

### On Windows (Current Development)
```bash
# Using GCC (MinGW) or WSL
make all
make run
```

### On Ubuntu/xv6 (Target Platform)
```bash
# Clone the repository
git clone <repo-url> kernel-panic-logger
cd kernel-panic-logger

# Build the project
make all

# Run the demonstration
make run
```

## Usage

### Log Buffer Example
```c
#include "log_buffer.h"

log_buffer_t log_buf;
log_buffer_init(&log_buf);

// Write logs
log_buffer_write(&log_buf, LOG_LEVEL_INFO, 0, "System initialized");
log_buffer_write(&log_buf, LOG_LEVEL_ERROR, 0, "Critical error occurred");

// Dump logs
log_buffer_dump(&log_buf);
```

### Crash Context Example
```c
#include "crash_context.h"

// When a panic occurs
crash_context_t ctx = crash_context_panic(
    14,  // Exception: Page Fault
    "Page Fault",
    "Invalid memory access",
    &log_buf
);

// Print crash details
crash_context_print(&ctx);

// Dump to custom buffer
char dump[2048];
crash_context_dump(&ctx, dump, sizeof(dump));
```

## API Reference

### Log Buffer Functions
- `log_buffer_init()` - Initialize the buffer
- `log_buffer_write()` - Write a log message
- `log_buffer_read()` - Read the next log entry
- `log_buffer_dump()` - Print all logs to console
- `log_buffer_get_total_entries()` - Get entry count
- `log_buffer_get_overflow_count()` - Get wrap-around count
- `log_buffer_clear()` - Clear all logs

### Crash Context Functions
- `crash_context_init()` - Initialize crash context
- `crash_context_capture_registers()` - Capture CPU state
- `crash_context_capture_stack_trace()` - Capture stack info
- `crash_context_capture_kernel_stack()` - Capture kernel stack bounds
- `crash_context_panic()` - Handle kernel panic with full capture
- `crash_context_print()` - Print crash report
- `crash_context_dump()` - Dump to buffer
- `crash_context_get_exception_name()` - Get exception description

## Windows Compatibility Note

This project is developed on Windows without xv6 support. The implementation includes:
- Portable C99 code
- Stub implementations for platform-specific features (register capture, stack unwinding)
- Full API design compatible with xv6 kernel architecture
- Demonstration program that works on any POSIX-compliant system

When porting to Ubuntu/xv6:
1. Update register capture functions to read interrupt frame
2. Implement real stack unwinding using frame pointers
3. Replace stub kernel stack information with actual bounds
4. Integrate with xv6 interrupt handler system

## Compilation for xv6

When compiling for xv6:
```bash
# Set xv6 compiler flags
CFLAGS = -fno-pic -static -fno-builtin -fno-strict-aliasing -Wall -Werror

# Include xv6 headers
CFLAGS += -I/path/to/xv6/include

# Use xv6 cross-compiler if available
CC = riscv64-unknown-elf-gcc  # Or appropriate target
```

## License

MIT License - See LICENSE file for details

## Contributing

This project is designed for xv6 kernel integration. Contributions should maintain:
1. POSIX C99 compatibility
2. Platform-agnostic design
3. No external dependencies
4. Efficient memory usage suitable for kernel context

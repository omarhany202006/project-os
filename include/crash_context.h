#ifndef CRASH_CONTEXT_H
#define CRASH_CONTEXT_H

#include <stdint.h>
#include <stddef.h>
#include "log_buffer.h"

/* Maximum stack trace depth to capture */
#define MAX_STACK_FRAMES 32

/* Size of crash dump buffer */
#define CRASH_DUMP_SIZE (64 * 1024)

/**
 * CPU register state at time of crash
 */
typedef struct {
    uint64_t rax, rbx, rcx, rdx;
    uint64_t rsi, rdi, rbp, rsp;
    uint64_t r8, r9, r10, r11;
    uint64_t r12, r13, r14, r15;
    uint64_t rip;              /* Instruction pointer */
    uint64_t rflags;           /* CPU flags */
    uint64_t cr0, cr2, cr3, cr4; /* Control registers */
} cpu_registers_t;

/**
 * Stack frame information for crash context
 */
typedef struct {
    uint64_t address;          /* Frame address */
    uint64_t return_address;   /* Return address of the frame */
    char symbol[128];          /* Symbol name (if available) */
} stack_frame_t;

/**
 * Complete crash context information
 */
typedef struct {
    uint64_t timestamp;                    /* When the crash occurred */
    uint32_t cpu_id;                       /* CPU that crashed */
    uint32_t exception_number;             /* Exception/interrupt number */
    char exception_name[64];               /* Human-readable exception name */
    char panic_message[512];               /* Panic message */
    
    /* CPU state at crash */
    cpu_registers_t registers;             /* CPU registers */
    
    /* Stack trace */
    uint32_t stack_frame_count;            /* Number of stack frames captured */
    stack_frame_t stack_frames[MAX_STACK_FRAMES]; /* Stack frames */
    
    /* Memory information */
    uint64_t fault_address;                /* Faulting memory address (if applicable) */
    uint64_t kernel_stack_start;           /* Start of kernel stack */
    uint64_t kernel_stack_end;             /* End of kernel stack */
    uint32_t kernel_stack_size;            /* Total kernel stack size */
    
    /* Process/Task information */
    uint32_t pid;                          /* Process ID (if applicable) */
    uint32_t tid;                          /* Thread ID */
    char process_name[64];                 /* Process/task name */
    
    /* System state */
    uint64_t uptime_ms;                    /* System uptime in milliseconds */
    uint32_t irq_level;                    /* IRQ level at crash */
    uint8_t in_interrupt;                  /* Was in interrupt handler */
} crash_context_t;

/**
 * Initialize crash context (zeros out structure)
 * @param ctx: pointer to crash context
 * @return 0 on success
 */
int crash_context_init(crash_context_t *ctx);

/**
 * Capture current CPU registers
 * @param ctx: pointer to crash context
 * @return 0 on success
 */
int crash_context_capture_registers(crash_context_t *ctx);

/**
 * Capture stack trace information
 * @param ctx: pointer to crash context
 * @param max_frames: maximum number of frames to capture
 * @return 0 on success
 */
int crash_context_capture_stack_trace(crash_context_t *ctx, uint32_t max_frames);

/**
 * Capture kernel stack information
 * @param ctx: pointer to crash context
 * @return 0 on success
 */
int crash_context_capture_kernel_stack(crash_context_t *ctx);

/**
 * Handle a kernel panic with full context capture
 * @param exception_number: exception/interrupt number
 * @param exception_name: human-readable exception name
 * @param message: panic message
 * @param log_buffer: pointer to kernel log buffer
 * @return crash_context_t structure with captured information
 */
crash_context_t crash_context_panic(uint32_t exception_number, 
                                     const char *exception_name,
                                     const char *message,
                                     log_buffer_t *log_buffer);

/**
 * Print crash context to console
 * @param ctx: pointer to crash context
 */
void crash_context_print(crash_context_t *ctx);

/**
 * Save crash dump to file/buffer
 * @param ctx: pointer to crash context
 * @param buffer: output buffer
 * @param buffer_size: size of output buffer
 * @return number of bytes written
 */
int crash_context_dump(crash_context_t *ctx, char *buffer, size_t buffer_size);

/**
 * Get exception name from exception number
 * @param exception_number: exception/interrupt number
 * @return pointer to exception name string
 */
const char* crash_context_get_exception_name(uint32_t exception_number);

#endif /* CRASH_CONTEXT_H */

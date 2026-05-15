#include "crash_context.h"
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/**
 * Exception names for x86-64 exceptions
 */
static const char* exception_names[] = {
    "Division by Zero",                    /* 0 */
    "Debug",                               /* 1 */
    "Non-Maskable Interrupt",              /* 2 */
    "Breakpoint",                          /* 3 */
    "Overflow",                            /* 4 */
    "Bound Range Exceeded",                /* 5 */
    "Invalid Opcode",                      /* 6 */
    "Device Not Available",                /* 7 */
    "Double Fault",                        /* 8 */
    "Coprocessor Segment Overrun",         /* 9 */
    "Invalid TSS",                         /* 10 */
    "Segment Not Present",                 /* 11 */
    "Stack-Segment Fault",                 /* 12 */
    "General Protection Fault",            /* 13 */
    "Page Fault",                          /* 14 */
    "Reserved",                            /* 15 */
    "x87 Floating-Point Exception",        /* 16 */
    "Alignment Check",                     /* 17 */
    "Machine Check",                       /* 18 */
    "SIMD Floating-Point Exception",       /* 19 */
    "Virtualization Exception"             /* 20 */
};

#define NUM_EXCEPTIONS (sizeof(exception_names) / sizeof(exception_names[0]))

/**
 * Get current timestamp
 */
static uint64_t get_timestamp_ms(void) {
    return (uint64_t)(clock() * 1000 / CLOCKS_PER_SEC);
}

static void append_dump(char *buffer, size_t buffer_size, size_t *written, const char *fmt, ...) {
    if (*written >= buffer_size) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buffer + *written, buffer_size - *written, fmt, args);
    va_end(args);

    if (n < 0) {
        return;
    }

    size_t needed = (size_t)n;
    if (needed >= buffer_size - *written) {
        *written = buffer_size - 1;
    } else {
        *written += needed;
    }
}

/**
 * Initialize crash context
 */
int crash_context_init(crash_context_t *ctx) {
    if (!ctx) {
        return -1;
    }
    
    memset(ctx, 0, sizeof(crash_context_t));
    return 0;
}

/**
 * Capture current CPU registers (platform-specific)
 * This is a stub implementation for Windows compatibility
 */
int crash_context_capture_registers(crash_context_t *ctx) {
    if (!ctx) {
        return -1;
    }
    
    /* On a real kernel, we would capture from the interrupt frame */
    /* For now, initialize with dummy values for demonstration */
    memset(&ctx->registers, 0, sizeof(cpu_registers_t));
    
    /* Mark that we attempted to capture */
    ctx->registers.rip = 0xDEADBEEF;
    ctx->registers.rsp = 0xCAFEBABE;
    
    return 0;
}

/**
 * Capture stack trace
 */
int crash_context_capture_stack_trace(crash_context_t *ctx, uint32_t max_frames) {
    if (!ctx || max_frames == 0) {
        return -1;
    }
    
    if (max_frames > MAX_STACK_FRAMES) {
        max_frames = MAX_STACK_FRAMES;
    }
    
    /* Stub implementation - would use stack unwinding in real kernel */
    ctx->stack_frame_count = 0;
    
    /* Create sample stack frames for demonstration */
    for (uint32_t i = 0; i < max_frames && i < 5; i++) {
        ctx->stack_frames[i].address = 0xFFFF800000000000 + (i * 0x100);
        ctx->stack_frames[i].return_address = 0xFFFF800000001000 + (i * 0x100);
        snprintf(ctx->stack_frames[i].symbol, sizeof(ctx->stack_frames[i].symbol),
                 "kernel_function_%u", i);
        ctx->stack_frame_count++;
    }
    
    return 0;
}

/**
 * Capture kernel stack information
 */
int crash_context_capture_kernel_stack(crash_context_t *ctx) {
    if (!ctx) {
        return -1;
    }
    
    /* Stub implementation */
    ctx->kernel_stack_start = 0xFFFF800000010000;
    ctx->kernel_stack_end = 0xFFFF800000020000;
    ctx->kernel_stack_size = ctx->kernel_stack_end - ctx->kernel_stack_start;
    
    return 0;
}

/**
 * Handle a kernel panic with full context capture
 */
crash_context_t crash_context_panic(uint32_t exception_number,
                                     const char *exception_name,
                                     const char *message,
                                     log_buffer_t *log_buffer) {
    crash_context_t ctx;
    crash_context_init(&ctx);
    
    /* Set basic panic information */
    ctx.timestamp = get_timestamp_ms();
    ctx.cpu_id = 0; /* Would be set by kernel */
    ctx.exception_number = exception_number;
    
    /* Set exception name */
    if (exception_name) {
        strncpy(ctx.exception_name, exception_name, sizeof(ctx.exception_name) - 1);
    } else {
        strncpy(ctx.exception_name, 
                crash_context_get_exception_name(exception_number),
                sizeof(ctx.exception_name) - 1);
    }
    
    /* Set panic message */
    if (message) {
        strncpy(ctx.panic_message, message, sizeof(ctx.panic_message) - 1);
    }
    
    /* Capture system state */
    crash_context_capture_registers(&ctx);
    crash_context_capture_stack_trace(&ctx, MAX_STACK_FRAMES);
    crash_context_capture_kernel_stack(&ctx);
    
    /* Log the panic to the kernel log buffer if provided */
    if (log_buffer) {
        char log_msg[512];
        size_t prefix_len;

        snprintf(log_msg, sizeof(log_msg),
                 "KERNEL PANIC: [%.63s] ",
                 ctx.exception_name);
        prefix_len = strlen(log_msg);
        snprintf(log_msg + prefix_len, sizeof(log_msg) - prefix_len,
                 "%.400s", ctx.panic_message);
        log_buffer_write(log_buffer, LOG_LEVEL_PANIC, ctx.cpu_id, log_msg);
    }
    
    ctx.uptime_ms = (uint64_t)(clock() * 1000 / CLOCKS_PER_SEC);
    ctx.irq_level = 0;
    ctx.in_interrupt = 1;
    ctx.pid = 0;
    ctx.tid = 0;
    strncpy(ctx.process_name, "kernel", sizeof(ctx.process_name) - 1);
    
    return ctx;
}

/**
 * Print crash context to console
 */
void crash_context_print(crash_context_t *ctx) {
    if (!ctx) {
        return;
    }
    
    printf("\n");
    printf("+================================================================+\n");
    printf("|                    KERNEL PANIC DETECTED                       |\n");
    printf("+================================================================+\n\n");
    
    printf("Exception: [%u] %s\n", ctx->exception_number, ctx->exception_name);
    printf("Message: %s\n", ctx->panic_message);
    printf("Timestamp: %" PRIu64 " ms\n", ctx->uptime_ms);
    printf("CPU ID: %u\n", ctx->cpu_id);
    printf("In Interrupt: %s\n", ctx->in_interrupt ? "Yes" : "No");
    
    printf("\n--- CPU REGISTERS ---\n");
    printf("RAX: 0x%016" PRIx64 "  RBX: 0x%016" PRIx64 "\n", ctx->registers.rax, ctx->registers.rbx);
    printf("RCX: 0x%016" PRIx64 "  RDX: 0x%016" PRIx64 "\n", ctx->registers.rcx, ctx->registers.rdx);
    printf("RSI: 0x%016" PRIx64 "  RDI: 0x%016" PRIx64 "\n", ctx->registers.rsi, ctx->registers.rdi);
    printf("RBP: 0x%016" PRIx64 "  RSP: 0x%016" PRIx64 "\n", ctx->registers.rbp, ctx->registers.rsp);
    printf("R8:  0x%016" PRIx64 "  R9:  0x%016" PRIx64 "\n", ctx->registers.r8, ctx->registers.r9);
    printf("R10: 0x%016" PRIx64 "  R11: 0x%016" PRIx64 "\n", ctx->registers.r10, ctx->registers.r11);
    printf("R12: 0x%016" PRIx64 "  R13: 0x%016" PRIx64 "\n", ctx->registers.r12, ctx->registers.r13);
    printf("R14: 0x%016" PRIx64 "  R15: 0x%016" PRIx64 "\n", ctx->registers.r14, ctx->registers.r15);
    printf("RIP: 0x%016" PRIx64 "  RFLAGS: 0x%016" PRIx64 "\n", ctx->registers.rip, ctx->registers.rflags);
    
    printf("\n--- CONTROL REGISTERS ---\n");
    printf("CR0: 0x%016" PRIx64 "  CR2: 0x%016" PRIx64 "\n", ctx->registers.cr0, ctx->registers.cr2);
    printf("CR3: 0x%016" PRIx64 "  CR4: 0x%016" PRIx64 "\n", ctx->registers.cr3, ctx->registers.cr4);
    
    printf("\n--- MEMORY INFORMATION ---\n");
    printf("Fault Address: 0x%016" PRIx64 "\n", ctx->fault_address);
    printf("Kernel Stack: 0x%016" PRIx64 " - 0x%016" PRIx64 " (Size: %u bytes)\n",
           ctx->kernel_stack_start, ctx->kernel_stack_end, ctx->kernel_stack_size);
    
    printf("\n--- STACK TRACE ---\n");
    if (ctx->stack_frame_count > 0) {
        for (uint32_t i = 0; i < ctx->stack_frame_count; i++) {
            printf("[%u] %s (0x%016" PRIx64 ") -> 0x%016" PRIx64 "\n",
                   i,
                   ctx->stack_frames[i].symbol,
                   ctx->stack_frames[i].address,
                   ctx->stack_frames[i].return_address);
        }
    } else {
        printf("No stack frames captured\n");
    }
    
    printf("\n--- PROCESS INFORMATION ---\n");
    printf("Process: %s (PID: %u, TID: %u)\n", ctx->process_name, ctx->pid, ctx->tid);
    
    printf("\n");
}

/**
 * Save crash dump to buffer
 */
int crash_context_dump(crash_context_t *ctx, char *buffer, size_t buffer_size) {
    if (!ctx || !buffer || buffer_size == 0) {
        return -1;
    }

    size_t written = 0;

    append_dump(buffer, buffer_size, &written, "KERNEL PANIC DUMP\n");
    append_dump(buffer, buffer_size, &written, "Exception: [%u] %s\n", ctx->exception_number, ctx->exception_name);
    append_dump(buffer, buffer_size, &written, "Message: %s\n", ctx->panic_message);
    append_dump(buffer, buffer_size, &written, "Timestamp: %" PRIu64 " ms\n", ctx->uptime_ms);
    append_dump(buffer, buffer_size, &written, "CPU ID: %u\n", ctx->cpu_id);
    append_dump(buffer, buffer_size, &written, "Instruction Pointer: 0x%016" PRIx64 "\n", ctx->registers.rip);
    append_dump(buffer, buffer_size, &written, "Stack Pointer: 0x%016" PRIx64 "\n", ctx->registers.rsp);
    append_dump(buffer, buffer_size, &written, "Kernel Stack: 0x%016" PRIx64 " - 0x%016" PRIx64 "\n",
                ctx->kernel_stack_start, ctx->kernel_stack_end);

    return (int)written;
}

/**
 * Get exception name from exception number
 */
const char* crash_context_get_exception_name(uint32_t exception_number) {
    if (exception_number < NUM_EXCEPTIONS) {
        return exception_names[exception_number];
    }
    return "Unknown Exception";
}

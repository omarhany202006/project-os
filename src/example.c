#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log_buffer.h"
#include "crash_context.h"

void demo_log_buffer(void) {
    printf("\n=== LOG BUFFER DEMONSTRATION ===\n\n");
    
    log_buffer_t log_buf;
    log_buffer_init(&log_buf);
    
    /* Write various log messages */
    log_buffer_write(&log_buf, LOG_LEVEL_INFO, 0, "Kernel initialized");
    log_buffer_write(&log_buf, LOG_LEVEL_DEBUG, 0, "Loading device drivers");
    log_buffer_write(&log_buf, LOG_LEVEL_INFO, 1, "CPU 1 online");
    log_buffer_write(&log_buf, LOG_LEVEL_WARN, 0, "Low memory condition detected");
    log_buffer_write(&log_buf, LOG_LEVEL_ERROR, 0, "Failed to mount root filesystem");
    log_buffer_write(&log_buf, LOG_LEVEL_DEBUG, 0, "Attempting recovery...");
    log_buffer_write(&log_buf, LOG_LEVEL_INFO, 0, "System recovered");
    
    /* Dump the log buffer */
    log_buffer_dump(&log_buf);
    
    printf("Total entries written: %u\n", log_buffer_get_total_entries(&log_buf));
    printf("Buffer overflows: %u\n", log_buffer_get_overflow_count(&log_buf));
}

void demo_crash_context(void) {
    printf("\n=== CRASH CONTEXT DEMONSTRATION ===\n\n");
    
    log_buffer_t log_buf;
    log_buffer_init(&log_buf);
    
    /* Simulate system operations */
    log_buffer_write(&log_buf, LOG_LEVEL_INFO, 0, "System running normally");
    log_buffer_write(&log_buf, LOG_LEVEL_DEBUG, 0, "Processing interrupt");
    
    /* Simulate a page fault */
    crash_context_t crash_ctx = crash_context_panic(
        14,  /* Page Fault exception number */
        "Page Fault",
        "Attempted to access invalid memory address 0xdeadbeef",
        &log_buf
    );
    
    /* Print crash context */
    crash_context_print(&crash_ctx);
    
    /* Dump to buffer */
    char dump_buffer[2048];
    int bytes = crash_context_dump(&crash_ctx, dump_buffer, sizeof(dump_buffer));
    printf("\n--- DUMP OUTPUT ---\n%s\n", dump_buffer);
    printf("--- END DUMP (%d bytes) ---\n\n", bytes);
}

void demo_exception_names(void) {
    printf("\n=== EXCEPTION NAMES REFERENCE ===\n\n");
    
    for (uint32_t i = 0; i <= 20; i++) {
        printf("[%2u] %s\n", i, crash_context_get_exception_name(i));
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║         KERNEL PANIC LOGGER - DEMONSTRATION PROGRAM             ║\n");
    printf("║                                                                ║\n");
    printf("║  Log Buffers & Crash Context Info for Panic Handling           ║\n");
    printf("║  Windows Implementation (Compatible with xv6 on Ubuntu)        ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    /* Run demonstrations */
    demo_exception_names();
    demo_log_buffer();
    demo_crash_context();
    
    printf("\n✓ All demonstrations completed successfully!\n");
    printf("Ready for deployment to Ubuntu/xv6 environment.\n\n");
    
    return 0;
}

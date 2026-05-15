#include "log_buffer.h"
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/* Global log buffer instance */
static log_buffer_t global_log_buffer;

/**
 * Get current timestamp (simplified - returns counter)
 */
static uint64_t get_timestamp(void) {
    return (uint64_t)clock();
}

/**
 * Initialize the kernel log buffer
 */
int log_buffer_init(log_buffer_t *buffer) {
    if (!buffer) {
        return -1;
    }

    memset(buffer->buffer, 0, LOG_BUFFER_SIZE);
    buffer->write_index = 0;
    buffer->read_index = 0;
    buffer->total_entries = 0;
    buffer->overflow_count = 0;
    buffer->is_initialized = 1;

    return 0;
}

/**
 * Write a message to the kernel log buffer
 */
int log_buffer_write(log_buffer_t *buffer, log_level_t level, uint32_t cpu_id, const char *message) {
    if (!buffer || !message || !buffer->is_initialized) {
        return -1;
    }

    log_entry_t entry;
    entry.timestamp = get_timestamp();
    entry.cpu_id = cpu_id;
    entry.level = level;
    entry.message_len = (uint32_t)strlen(message);

    if (entry.message_len >= LOG_MSG_MAX_SIZE) {
        entry.message_len = LOG_MSG_MAX_SIZE - 1;
    }

    strncpy(entry.message, message, entry.message_len);
    entry.message[entry.message_len] = '\0';

    size_t entry_size = sizeof(log_entry_t);
    uint32_t space_available = LOG_BUFFER_SIZE - buffer->write_index;

    if (space_available < entry_size) {
        buffer->write_index = 0;
        buffer->overflow_count++;
    }

    memcpy(&buffer->buffer[buffer->write_index], &entry, entry_size);
    buffer->write_index += entry_size;
    buffer->total_entries++;

    if (buffer->write_index >= LOG_BUFFER_SIZE) {
        buffer->write_index = 0;
        buffer->overflow_count++;
    }

    return (int)entry_size;
}

/**
 * Read the next log entry from the buffer
 */
int log_buffer_read(log_buffer_t *buffer, log_entry_t *entry) {
    if (!buffer || !entry || !buffer->is_initialized) {
        return -1;
    }

    if (buffer->read_index >= buffer->write_index) {
        return 0;
    }

    size_t entry_size = sizeof(log_entry_t);
    if (buffer->read_index + entry_size > LOG_BUFFER_SIZE) {
        return 0;
    }

    memcpy(entry, &buffer->buffer[buffer->read_index], entry_size);
    buffer->read_index += entry_size;

    return 1;
}

/**
 * Get current state of the log buffer
 */
uint32_t log_buffer_get_total_entries(log_buffer_t *buffer) {
    if (!buffer) {
        return 0;
    }
    return buffer->total_entries;
}

/**
 * Get overflow count
 */
uint32_t log_buffer_get_overflow_count(log_buffer_t *buffer) {
    if (!buffer) {
        return 0;
    }
    return buffer->overflow_count;
}

/**
 * Clear the log buffer
 */
int log_buffer_clear(log_buffer_t *buffer) {
    if (!buffer) {
        return -1;
    }

    memset(buffer->buffer, 0, LOG_BUFFER_SIZE);
    buffer->write_index = 0;
    buffer->read_index = 0;
    buffer->total_entries = 0;
    buffer->overflow_count = 0;

    return 0;
}

/**
 * Get log level name
 */
static const char* get_log_level_name(log_level_t level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO: return "INFO";
        case LOG_LEVEL_WARN: return "WARN";
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_PANIC: return "PANIC";
        default: return "UNKNOWN";
    }
}

/**
 * Dump entire log buffer to console
 */
void log_buffer_dump(log_buffer_t *buffer) {
    if (!buffer || !buffer->is_initialized) {
        printf("Log buffer not initialized\n");
        return;
    }

    printf("\n========== KERNEL LOG BUFFER DUMP ==========\n");
    printf("Total entries: %u\n", buffer->total_entries);
    printf("Overflow count: %u\n", buffer->overflow_count);
    printf("==========================================\n\n");

    log_entry_t entry;
    uint32_t count = 0;

    /* Reset read index to beginning */
    uint32_t saved_read_index = buffer->read_index;
    buffer->read_index = 0;

    while (log_buffer_read(buffer, &entry) == 1) {
        printf("[%" PRIu64 "] [CPU %u] [%s] %s\n",
               entry.timestamp,
               entry.cpu_id,
               get_log_level_name(entry.level),
               entry.message);
        count++;
    }

    printf("\n========= END OF LOG DUMP (%u entries) =========\n\n", count);
    buffer->read_index = saved_read_index;
}

/**
 * Get global log buffer instance
 */
log_buffer_t* log_buffer_get_global(void) {
    static int initialized = 0;
    if (!initialized) {
        log_buffer_init(&global_log_buffer);
        initialized = 1;
    }
    return &global_log_buffer;
}

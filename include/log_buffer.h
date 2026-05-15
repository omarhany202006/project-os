#ifndef LOG_BUFFER_H
#define LOG_BUFFER_H

#include <stdint.h>
#include <stddef.h>

/* Maximum size of a single log message */
#define LOG_MSG_MAX_SIZE 256

/* Total size of the circular ring buffer (256 KB) */
#define LOG_BUFFER_SIZE (256 * 1024)

/* Number of log entries that can be stored */
#define LOG_BUFFER_ENTRIES (LOG_BUFFER_SIZE / LOG_MSG_MAX_SIZE)

/**
 * Log levels for categorizing kernel messages
 */
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_ERROR = 3,
    LOG_LEVEL_PANIC = 4
} log_level_t;

/**
 * Structure representing a single log entry
 */
typedef struct {
    uint64_t timestamp;          /* Timestamp when log was written (in ticks) */
    uint32_t cpu_id;             /* CPU ID that generated the log */
    log_level_t level;           /* Log level */
    uint32_t message_len;        /* Length of the message */
    char message[LOG_MSG_MAX_SIZE]; /* Log message content */
} log_entry_t;

/**
 * Circular ring buffer structure for kernel logs
 */
typedef struct {
    uint8_t buffer[LOG_BUFFER_SIZE]; /* Raw buffer storage */
    uint32_t write_index;           /* Current write position */
    uint32_t read_index;            /* Current read position */
    uint32_t total_entries;         /* Total entries written (for overflow tracking) */
    uint32_t overflow_count;        /* Number of times buffer wrapped around */
    uint8_t is_initialized;         /* Initialization flag */
} log_buffer_t;

/**
 * Initialize the kernel log buffer
 * @return 0 on success, -1 on failure
 */
int log_buffer_init(log_buffer_t *buffer);

/**
 * Write a message to the kernel log buffer
 * @param buffer: pointer to log buffer
 * @param level: log level
 * @param cpu_id: ID of the CPU generating the log
 * @param message: NULL-terminated message string
 * @return number of bytes written, -1 on failure
 */
int log_buffer_write(log_buffer_t *buffer, log_level_t level, uint32_t cpu_id, const char *message);

/**
 * Read the next log entry from the buffer
 * @param buffer: pointer to log buffer
 * @param entry: pointer to store the read entry
 * @return 1 if entry was read, 0 if no more entries, -1 on error
 */
int log_buffer_read(log_buffer_t *buffer, log_entry_t *entry);

/**
 * Get current state of the log buffer
 * @param buffer: pointer to log buffer
 * @return total number of entries written
 */
uint32_t log_buffer_get_total_entries(log_buffer_t *buffer);

/**
 * Get overflow count
 * @param buffer: pointer to log buffer
 * @return number of times buffer wrapped around
 */
uint32_t log_buffer_get_overflow_count(log_buffer_t *buffer);

/**
 * Clear the log buffer
 * @param buffer: pointer to log buffer
 * @return 0 on success
 */
int log_buffer_clear(log_buffer_t *buffer);

/**
 * Dump entire log buffer to console (for debugging)
 * @param buffer: pointer to log buffer
 */
void log_buffer_dump(log_buffer_t *buffer);

#endif /* LOG_BUFFER_H */

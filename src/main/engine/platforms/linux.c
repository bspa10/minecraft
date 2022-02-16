#include "detector.h"

#ifdef TOE_PLATFORM_LINUX
#include "platform.h"
#include "../logger.h"
#include "../runtime.h"

// #######################################################
//                        Lifecycle
// #######################################################
void toe_platform_initialize() {
    TOE_LOG_DEBUG("[platform.h] Initializing")
    struct TOERuntime * runtime = toe_runtime_get();
    runtime->platform.name = "LINUX";
}

void toe_platform_destroy() {
    TOE_LOG_DEBUG("[platform.h] Destroying")

}
// #######################################################
//                        Memory
// #######################################################
void check_size(uint64_t size) {
    if (size <= 0) TOE_LOG_FATAL("[platform.h] Invalid block size [%d]", size)
}

void * toe_platform_memory_allocate(uint64_t size) {
    check_size(size);
    void * block = malloc(size);

    if (block == NULL)
        TOE_LOG_FATAL("[platform.h] toe_platform_memory_allocate(...) : Unable to allocate memory")

    toe_platform_memory_zero(block, size);
    return block;
}

void check_block_is_null(cstring_t name, void * block) {
    if (block == NULL)
        TOE_LOG_FATAL("[platform.h] %s is NULL", name)
}

void toe_platform_memory_free(void * block) {
    check_block_is_null("Block", block);
    free(block);
}

#include <string.h>
void * toe_platform_memory_zero(void * block, uint64_t  size) {
    check_size(size);
    check_block_is_null("Block", block);

    return toe_platform_memory_set(block, 0, size);
}

void * toe_platform_memory_copy(void * source, void * destination, uint64_t  size) {
    check_size(size);
    check_block_is_null("Source", source);
    check_block_is_null("Destination", destination);

    if (source == NULL) TOE_LOG_FATAL("[platform.h] toe_platform_memory_copy(..) : Block is NULL")
    if (destination == NULL) TOE_LOG_FATAL("[platform.h] toe_platform_memory_copy(..) : Block is NULL")

    return memcpy(destination, source, size);
}

void * toe_platform_memory_set(void * block, int32_t value, uint64_t  size) {
    check_size(size);
    check_block_is_null("Block", block);

    return memset(block, value, size);
}
// #######################################################
//                        Logging
// #######################################################
#include <stdio.h>
#include <time.h>

char log_message_buffer[PLATFORM_LOG_SIZE];
const uint8_t log_level_color[TOE_LOG_LEVEL_TRACE + 1] = {31, 31, 33, 32, 36, 35};
cstring_t log_level_name[TOE_LOG_LEVEL_TRACE + 1] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE"};

void toe_platform_log(TOELogLevel level, cstring_t filename, uint64_t line, cstring_t message, ...) {
    memset(log_message_buffer, 0, sizeof(log_message_buffer));

    __builtin_va_list ptr;
    va_start(ptr, message);
    vsnprintf(log_message_buffer, PLATFORM_LOG_SIZE, message, ptr);
    va_end(ptr);

    printf("\033[%dm%.6f [%s] (%s:%lu) %s\033[0m\n",
           log_level_color[level],
           (float) clock() / CLOCKS_PER_SEC,
           log_level_name[level],
           filename,
           line,
           log_message_buffer
    );
}
#endif

#include "memory.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

#define KB 1024
#define MB 1024 * KB
#define GB 1024 * MB

struct TOEMemory {
    uint64_t allocated;
    uint64_t pool[TOE_MEMORY_MAX];
} registry;

string_t to_string(uint64_t size) {

    char buffer[50];
    char unit[4] = "XiB";
    float amount = 1.0f;

    if (size >= GB) {
        unit[0] = 'G';
        amount = size / (GB * 1.0);
    } else if (size >= MB) {
        unit[0] = 'M';
        amount = size / (MB * 1.0);
    } else if (size >= KB) {
        unit[0] = 'K';
        amount = size / (KB * 1.0);
    } else {
        unit[0] = 'B';
        unit[1] = 0;

        amount = (float) size;
    }

    snprintf(buffer, 50, "%.2f %s", amount, unit);

    uint64_t length = strlen(buffer);
    string_t copy = toe_platform_memory_allocate(length + 1);
    toe_memory_copy(buffer, copy, length + 1);

    return copy;
}

void print_memory_trace(uint64_t size) {
    string_t str_size = to_string(size);
    string_t str_total = to_string(registry.allocated);

    TOE_LOG_TRACE("[memory.h] registry->allocated: (+%s) %s", str_size, str_total)

    toe_platform_memory_free(str_size);
    toe_platform_memory_free(str_total);
}

void toe_memory_initialize() {
    TOE_LOG_DEBUG("[memory.h] Initializing")
    uint64_t base = sizeof (struct TOEMemory);

    registry.allocated = base;
    registry.pool[TOE_MEMORY_APPLICATION] = base;

    print_memory_trace(base);
}

void toe_memory_destroy() {
    TOE_LOG_DEBUG("[memory.h] Destroying")
    uint64_t base = sizeof (struct TOEMemory);

    registry.allocated -= base;
    print_memory_trace(base);

    if (registry.allocated != 0)
        TOE_LOG_ERROR("[memory.h] Memory Leak of %d", registry.allocated)
}

TOEAPI void * toe_memory_allocate(uint64_t size, enum TOEMemoryType tag) {
    void * block = toe_platform_memory_allocate(size);

    registry.allocated += size;
    registry.pool[tag] += size;
    print_memory_trace(size);

    return block;
}

TOEAPI void toe_memory_free(void * block, uint64_t  size, enum TOEMemoryType tag) {
    toe_platform_memory_free(block);

    registry.allocated -= size;
    registry.pool[tag] -= size;
    print_memory_trace(size);
}

TOEAPI void * toe_memory_zero(void * block, uint64_t  size) {
    return toe_platform_memory_zero(block, size);
}

TOEAPI void * toe_memory_copy(void * source, void * destination, uint64_t size) {
    return toe_platform_memory_copy(source, destination, size);
}

TOEAPI void * toe_memory_set(void * block, int32_t value, uint64_t size) {
    return toe_platform_memory_set(block, value, size);
}

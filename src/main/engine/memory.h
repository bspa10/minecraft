#pragma once

#include "core/types.h"

enum TOEMemoryType {
    TOE_MEMORY_UNKNOWN,
    TOE_MEMORY_ENGINE,
    TOE_MEMORY_APPLICATION,
    TOE_MEMORY_STRING,

    TOE_MEMORY_MAX
} ;

// #######################################################
//                        Lifecycle
// #######################################################
void toe_memory_initialize();
void toe_memory_destroy();
// #######################################################
//                        API
// #######################################################
void * toe_memory_allocate(uint64_t size, enum TOEMemoryType type);
void   toe_memory_free(void * block, uint64_t  size, enum TOEMemoryType type);
void * toe_memory_zero(void * block, uint64_t  size);
void * toe_memory_copy(void * source, void * destination, uint64_t  size);
void * toe_memory_set(void * block, int32_t value, uint64_t  size);

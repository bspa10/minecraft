#pragma once

#include "../core/types.h"
#define PLATFORM_LOG_SIZE 32001

// #######################################################
//                        Lifecycle
// #######################################################
void toe_platform_initialize();
void toe_platform_destroy();
// #######################################################
//                        Memory
// #######################################################
void * toe_platform_memory_allocate(uint64_t size);
void toe_platform_memory_free(void * block);
void * toe_platform_memory_zero(void * block, uint64_t  size);
void * toe_platform_memory_copy(void * source, void * destination, uint64_t  size);
void * toe_platform_memory_set(void * block, int32_t value, uint64_t  size);
// #######################################################
//                        Logging
// #######################################################
void toe_platform_log(TOELogLevel level, cstring_t filename, uint64_t line, cstring_t message, ...);

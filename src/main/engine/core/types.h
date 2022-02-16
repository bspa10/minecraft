#pragma once

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

typedef const char * cstring_t;
typedef       char * string_t;

typedef enum {
    TOE_LOG_LEVEL_FATAL,
    TOE_LOG_LEVEL_ERROR,
    TOE_LOG_LEVEL_WARN ,
    TOE_LOG_LEVEL_INFO,
    TOE_LOG_LEVEL_DEBUG,
    TOE_LOG_LEVEL_TRACE
} TOELogLevel;

typedef enum {
    TRUE = 1,
    FALSE = 0
} TOEBoolean;

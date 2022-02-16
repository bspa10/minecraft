#pragma once
#include "platforms/detector.h"

#ifdef TOE_PLATFORM_LINUX
#include "core/defines.h"
#include "platforms/platform.h"

#include <signal.h>
#include <stdlib.h>

#define TOE_LOG_FATAL(message, ...) {                                                                               \
                                        toe_platform_log(TOE_LOG_LEVEL_FATAL, __FILE_NAME__, __LINE__, message, ##__VA_ARGS__);   \
                                        raise(SIGTRAP);                                                             \
                                        exit(9);                                                                    \
                                    }

#define TOE_LOG_ERROR(message, ...) toe_platform_log(TOE_LOG_LEVEL_ERROR, __FILE_NAME__, __LINE__, message, ##__VA_ARGS__);
#define TOE_LOG_WARN(message, ...) toe_platform_log(TOE_LOG_LEVEL_WARN, __FILE_NAME__, __LINE__, message, ##__VA_ARGS__);
#define TOE_LOG_INFO(message, ...) toe_platform_log(TOE_LOG_LEVEL_INFO, __FILE_NAME__, __LINE__, message, ##__VA_ARGS__);
#define TOE_LOG_DEBUG(message, ...) toe_platform_log(TOE_LOG_LEVEL_DEBUG, __FILE_NAME__, __LINE__, message, ##__VA_ARGS__);
#define TOE_LOG_TRACE(message, ...) toe_platform_log(TOE_LOG_LEVEL_TRACE, __FILE_NAME__, __LINE__, message, ##__VA_ARGS__);

#endif

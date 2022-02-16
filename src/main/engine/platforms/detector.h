#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define TOE_PLATFORM_WINDOWS TRUE
        #ifndef _WIN64
            #error "64-bit is required on Windows!"
        #endif
    #elif defined(__linux__) || defined(__gnu_linux__)
        // Linux OS
        #define TOE_PLATFORM_LINUX
        #if defined(__ANDROID__)
        #define TOE_PLATFORM_ANDROID
    #endif
#elif defined(__unix__)
// Catch anything not caught by the above.
    #define TOE_PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
    // Posix
    #define TOE_PLATFORM_POSIX
#elif __APPLE__
    // Apple platforms
    #define TOE_PLATFORM_APPLE
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
        #define TOE_PLATFORM_IOS
        #define TOE_PLATFORM_IOS_SIMULATOR
    #elif TARGET_OS_IPHONE
        // iOS device
        #define TOE_PLATFORM_IOS
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
        #error "Unknown Apple platform"
    #endif
#else
    #error "Unknown platform!"
#endif

#pragma once

#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
    #ifdef BUILDING_DLL
        #ifdef __GNUC__
            #define TOEAPI __attribute__ ((dllexport))
        #else
            #define TOEAPI __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
        #endif
    #else
        #ifdef __GNUC__
            #define TOEAPI __attribute__ ((dllimport))
        #else
            #define TOEAPI __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
        #endif
    #endif
#else
    #if __GNUC__ >= 4
        #define TOEAPI __attribute__ ((visibility ("default")))
    #else
        #define TOEAPI
    #endif
#endif


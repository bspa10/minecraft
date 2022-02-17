#pragma once

#include "core/types.h"

struct TOERuntime {
    struct {
        uint32_t  handle;
        uint16_t  rate;
        uint16_t  rotation;

        void * intrinsics;

        struct {
            uint16_t  width;
            uint16_t  height;
        } resolution;
    } monitor;

    struct {
        uint32_t handle;
        cstring_t title;
        struct {
            int16_t x;
            int16_t y;
        } position;
        struct {
            int16_t width;
            int16_t height;
        } size;
    } window;

    struct {
        cstring_t name;
        void * intrinsics;
    } platform;
};

// #######################################################
//                        Lifecycle
// #######################################################
void toe_runtime_initialize();
void toe_runtime_destroy();
// #######################################################
//                        API
// #######################################################
struct TOERuntime * toe_runtime_get();

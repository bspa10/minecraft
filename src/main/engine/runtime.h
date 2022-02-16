#pragma once

#include "core/types.h"

struct TOERuntime {
    struct {
        cstring_t title;
        struct {
            uint16_t x;
            uint16_t y;
        } position;
        struct {
            uint16_t width;
            uint16_t height;
        } size;
    } window;

    struct {
        cstring_t name;
        void * intrinsics;
    } platform;
};

void toe_runtime_initialize();
void toe_runtime_destroy();

struct TOERuntime * toe_runtime_get();

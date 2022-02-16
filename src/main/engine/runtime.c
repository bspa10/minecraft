#include <stddef.h>
#include "runtime.h"
#include "memory.h"
#include "logger.h"

struct TOERuntime * runtime;

void toe_runtime_initialize() {
    TOE_LOG_DEBUG("[runtime.h] Initializing")
    runtime = toe_memory_allocate(sizeof(struct TOERuntime), TOE_MEMORY_ENGINE);

    runtime->window.title = "Game";
    runtime->window.position.x = 0;
    runtime->window.position.y = 0;
    runtime->window.size.width = 640;
    runtime->window.size.height = 480;

    runtime->platform.name = "undefined";
}

void toe_runtime_destroy() {
    TOE_LOG_DEBUG("[runtime.h] Destroying")

    if (runtime->platform.intrinsics != NULL)
        TOE_LOG_FATAL("[runtime.h] runtime->platform.intrinsics is not NULL")


    toe_memory_free(runtime, sizeof (struct TOERuntime), TOE_MEMORY_ENGINE);
    runtime = NULL;
}

struct TOERuntime * toe_runtime_get() {
    if (runtime == NULL)
        TOE_LOG_FATAL("[runtime.h] runtime is NULL")

    return runtime;
}


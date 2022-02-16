#include "engine/platforms/platform.h"
#include "engine/memory.h"
#include "engine/runtime.h"

int main() {
    toe_memory_initialize();
    toe_runtime_initialize();
    toe_platform_initialize();

    toe_platform_destroy();
    toe_runtime_destroy();
    toe_memory_destroy();

    return 0;
}

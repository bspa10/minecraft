#include "engine/platforms/platform.h"
#include "engine/memory.h"
#include "engine/runtime.h"
#include "engine/window.h"

int main() {
    toe_memory_initialize();
    toe_runtime_initialize();
    toe_platform_initialize();
    toe_window_initialize();

    toe_window_show();

    sleep(1);
    toe_window_hide();

    toe_window_destroy();
    toe_platform_destroy();
    toe_runtime_destroy();
    toe_memory_destroy();

    return 0;
}

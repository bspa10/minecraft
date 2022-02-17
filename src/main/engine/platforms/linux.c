#include "detector.h"

#ifdef TOE_PLATFORM_LINUX
#include "platform.h"
#include "../memory.h"
#include "../logger.h"
#include "../runtime.h"

#include "../../vendor/xcb/xcbutils.h"

struct TOEPlatform {
    xcb_connection_t    * connection;
    xcb_generic_error_t * error;

    struct {
        uint32_t wm_protocols;
        uint32_t wm_delete_window;
    } atom;
} ;

struct TOEXCBMonitor {
    uint8_t  depth;
    uint32_t  visualid;
    uint32_t  black_pixel;
};

// #######################################################
//                        Lifecycle
// #######################################################
void toe_platform_initialize() {
    TOE_LOG_DEBUG("[platform.h] Initializing")
    struct TOERuntime * runtime = toe_runtime_get();

    runtime->platform.name = "LINUX";
    runtime->platform.intrinsics = toe_memory_allocate(sizeof(struct TOEPlatform), TOE_MEMORY_ENGINE);
}

void toe_platform_destroy() {
    TOE_LOG_DEBUG("[platform.h] Destroying")
    struct TOERuntime * runtime = toe_runtime_get();

    toe_memory_free(runtime->platform.intrinsics, sizeof(struct TOEPlatform), TOE_MEMORY_ENGINE);
    runtime->platform.intrinsics = NULL;
}
// #######################################################
//                        Memory
// #######################################################
void check_size(uint64_t size) {
    if (size <= 0) TOE_LOG_FATAL("[platform.h] Invalid block size [%d]", size)
}

void * toe_platform_memory_allocate(uint64_t size) {
    check_size(size);
    void * block = malloc(size);

    if (block == NULL)
        TOE_LOG_FATAL("[platform.h] toe_platform_memory_allocate(...) : Unable to allocate memory")

    toe_platform_memory_zero(block, size);
    return block;
}

void check_block_is_null(cstring_t name, void * block) {
    if (block == NULL)
        TOE_LOG_FATAL("[platform.h] %s is NULL", name)
}

void toe_platform_memory_free(void * block) {
    check_block_is_null("Block", block);
    free(block);
}

#include <string.h>
void * toe_platform_memory_zero(void * block, uint64_t  size) {
    check_size(size);
    check_block_is_null("Block", block);

    return toe_platform_memory_set(block, 0, size);
}

void * toe_platform_memory_copy(void * source, void * destination, uint64_t  size) {
    check_size(size);
    check_block_is_null("Source", source);
    check_block_is_null("Destination", destination);

    if (source == NULL) TOE_LOG_FATAL("[platform.h] toe_platform_memory_copy(..) : Block is NULL")
    if (destination == NULL) TOE_LOG_FATAL("[platform.h] toe_platform_memory_copy(..) : Block is NULL")

    return memcpy(destination, source, size);
}

void * toe_platform_memory_set(void * block, int32_t value, uint64_t  size) {
    check_size(size);
    check_block_is_null("Block", block);

    return memset(block, value, size);
}
// #######################################################
//                        Logging
// #######################################################
#include <stdio.h>
#include <time.h>

char log_message_buffer[PLATFORM_LOG_SIZE];
const uint8_t log_level_color[TOE_LOG_LEVEL_TRACE + 1] = {31, 31, 33, 32, 36, 35};
cstring_t log_level_name[TOE_LOG_LEVEL_TRACE + 1] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE"};

void toe_platform_log(TOELogLevel level, cstring_t filename, uint64_t line, cstring_t message, ...) {
    memset(log_message_buffer, 0, sizeof(log_message_buffer));

    __builtin_va_list ptr;
    va_start(ptr, message);
    vsnprintf(log_message_buffer, PLATFORM_LOG_SIZE, message, ptr);
    va_end(ptr);

    printf("\033[%dm%.6f [%s] (%s:%lu) %s\033[0m\n",
           log_level_color[level],
           (float) clock() / CLOCKS_PER_SEC,
           log_level_name[level],
           filename,
           line,
           log_message_buffer
    );
}
// #######################################################
//                        Window
// #######################################################
#include "../window.h"

void toe_window_initialize() {
    TOE_LOG_DEBUG("[window.h] Initializing")
    struct TOERuntime * runtime = toe_runtime_get();
    struct TOEPlatform * platform = runtime->platform.intrinsics;
    // ###########################
    // Connection
    // ###########################
    {
        platform->connection = xcb_connect(NULL, NULL);
        if (xcb_connection_has_error(platform->connection))
            TOE_LOG_FATAL("X11 [XBC] :: Error opening display.")
    }
    // ###########################
    // Monitor
    // ###########################
    {
        const struct xcb_setup_t *xcb_setup = xcb_get_setup(platform->connection);
        xcb_screen_t *xcb_screen = xcb_setup_roots_iterator(xcb_setup).data;

        runtime->monitor.handle = xcb_screen->root;
        runtime->monitor.resolution.width = xcb_screen->width_in_pixels;
        runtime->monitor.resolution.height = xcb_screen->height_in_pixels;
        runtime->monitor.intrinsics = toe_memory_allocate(sizeof(struct TOEXCBMonitor), TOE_MEMORY_PLATFORM);

        ((struct TOEXCBMonitor *) runtime->monitor.intrinsics)->black_pixel = xcb_screen->black_pixel;
        ((struct TOEXCBMonitor *) runtime->monitor.intrinsics)->visualid = xcb_screen->root_visual;
        ((struct TOEXCBMonitor *) runtime->monitor.intrinsics)->depth = xcb_screen->root_depth;

        xcb_randr_get_screen_info_cookie_t cookie = xcb_randr_get_screen_info(platform->connection,
                                                                              runtime->monitor.handle);
        xcb_randr_get_screen_info_reply_t *info = xcb_randr_get_screen_info_reply(
                platform->connection,
                cookie,
                &platform->error);

        toe_xcb_check_error(platform->error, "xcb_randr_get_screen_info");

        runtime->monitor.rotation = info->rotation;
        runtime->monitor.rate = info->rate;
        toe_platform_memory_free(info);
    }
    // ###########################
    // Window
    // ###########################
    {
        uint32_t values[2] = {
                ((struct TOEXCBMonitor *) runtime->monitor.intrinsics)->black_pixel,
                XCB_EVENT_MASK_EXPOSURE |

                XCB_EVENT_MASK_POINTER_MOTION |
                XCB_EVENT_MASK_BUTTON_PRESS |
                XCB_EVENT_MASK_BUTTON_RELEASE |

                XCB_EVENT_MASK_KEY_PRESS |
                XCB_EVENT_MASK_KEY_RELEASE |

                XCB_EVENT_MASK_FOCUS_CHANGE |
                XCB_EVENT_MASK_ENTER_WINDOW |
                XCB_EVENT_MASK_LEAVE_WINDOW |
                XCB_EVENT_MASK_PROPERTY_CHANGE
        };

        runtime->window.handle = xcb_generate_id(platform->connection);
        xcb_void_cookie_t cookie = xcb_create_window_checked(
                platform->connection,
                XCB_COPY_FROM_PARENT,
                runtime->window.handle,
                runtime->monitor.handle,
                runtime->window.position.x,
                runtime->window.position.y,
                runtime->window.size.width,
                runtime->window.size.height,
                0,
                XCB_WINDOW_CLASS_INPUT_OUTPUT,
                ((struct TOEXCBMonitor *) runtime->monitor.intrinsics)->visualid,
                XCB_CW_EVENT_MASK | XCB_CW_BACK_PIXEL,
                values
        );
        xcb_generic_error_t *error = xcb_request_check(platform->connection, cookie);
        toe_xcb_check_error(error, "xcb_create_window_checked");
    }
    // ###########################
    // Platform Events
    // ###########################
    {
        platform->atom.wm_protocols = toe_xcb_get_atom(platform->connection, "WM_PROTOCOLS", platform->error);
        platform->atom.wm_delete_window = toe_xcb_get_atom(platform->connection, "WM_DELETE_WINDOW", platform->error);

        xcb_change_property(
                platform->connection,
                XCB_PROP_MODE_REPLACE,
                runtime->window.handle,
                platform->atom.wm_protocols,
                4,
                32,
                1,
                &platform->atom.wm_delete_window
        );

        toe_xcb_check_error(platform->error, "xcb_change_property[WM_PROTOCOLS]");
        toe_window_set_title(runtime->window.title);
    }
}

void toe_window_destroy() {
    TOE_LOG_DEBUG("[window.h] Destroying")
    struct TOERuntime * runtime = toe_runtime_get();
    struct TOEPlatform * platform = runtime->platform.intrinsics;

    xcb_disconnect(platform->connection);
    platform->connection = NULL;

    toe_memory_free(runtime->monitor.intrinsics, sizeof(struct TOEXCBMonitor), TOE_MEMORY_PLATFORM);
    runtime->monitor.intrinsics = NULL;
}

void toe_window_hide() {
    struct TOERuntime * runtime = toe_runtime_get();
    struct TOEPlatform * platform = runtime->platform.intrinsics;

    xcb_void_cookie_t cookie = xcb_unmap_window_checked(platform->connection, runtime->window.handle);
    xcb_generic_error_t * error = xcb_request_check(platform->connection, cookie);
    if (error != NULL) {
        toe_xcb_check_error(error, "xcb_unmap_window_checked");
        toe_platform_memory_free(error);
    }
}

void toe_window_show() {
    struct TOERuntime * runtime = toe_runtime_get();
    struct TOEPlatform * platform = runtime->platform.intrinsics;

    xcb_void_cookie_t cookie = xcb_map_window_checked(platform->connection, runtime->window.handle);
    xcb_generic_error_t * error = xcb_request_check(platform->connection, cookie);
    if (error != NULL) {
        toe_xcb_check_error(error, "xcb_map_window_checked");
        toe_platform_memory_free(error);
    }
}

void toe_window_refresh() {
    struct TOERuntime * runtime = toe_runtime_get();
    struct TOEPlatform * platform = runtime->platform.intrinsics;
    xcb_flush (platform->connection);
}

void toe_window_set_title(cstring_t title) {
    struct TOERuntime * runtime = toe_runtime_get();
    struct TOEPlatform * platform = runtime->platform.intrinsics;

    xcb_change_property(
            platform->connection,
            XCB_PROP_MODE_REPLACE,
            runtime->window.handle,
            XCB_ATOM_WM_NAME,
            XCB_ATOM_STRING,
            8, // (8-bit, 16-bit or 32-bit)
            strlen(title),
            title
    );
}
#endif

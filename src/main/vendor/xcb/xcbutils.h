#pragma once

#include <xcb/xcb.h>
#include <xcb/randr.h>
#include "../../engine/core/types.h"

uint32_t toe_xcb_get_atom(xcb_connection_t * connection, cstring_t name, xcb_generic_error_t * error);
void     toe_xcb_check_error(xcb_generic_error_t * error, cstring_t message);

#pragma once

#include "core/types.h"

// #######################################################
//                        Lifecycle
// #######################################################
void toe_window_initialize();
void toe_window_destroy();
// #######################################################
//                        API
// #######################################################
void toe_window_hide();
void toe_window_show();
void toe_window_refresh();
void toe_window_set_title(cstring_t title);

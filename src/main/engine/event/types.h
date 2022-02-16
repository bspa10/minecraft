#pragma once

enum TOEEventCategory {
    EVENT_CATEGORY_APPLICATION = 1 << 0,
    EVENT_CATEGORY_WINDOW = 1 << 1,
    EVENT_CATEGORY_INPUT = 1 << 2
} ;

enum TOEEventType {
    EVENT_TYPE_WINDOW_CLOSE = 1 << 0,
    EVENT_TYPE_KEY_PRESSED = 1 << 1,
    EVENT_TYPE_KEY_RELEASED = 1 << 2
} ;

struct TOEEvent {
    enum TOEEventType       type;
    enum TOEEventCategory   category;
    void                    * data;
};

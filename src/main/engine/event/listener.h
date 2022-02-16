#pragma once

#include "types.h"

void toe_event_subscribe(enum TOEEventCategory category, enum TOEEventType type, void (* function)(struct TOEEvent));

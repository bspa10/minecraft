#pragma once

#include "../core/types.h"

struct TOEList {
    cstring_t type;
    uint64_t allocated;

    uint16_t capacity;
    uint16_t length;

    void     * data;
};


struct TOEList * toe_list_create();
void toe_list_destroy(struct TOEList * list);

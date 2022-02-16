#pragma once

#include "../core/types.h"

typedef struct {
    cstring_t type;
    uint64_t allocated;

    uint16_t capacity;
    uint16_t length;

    void     * data;
} TOEList;


TOEList * toe_list_create();
void toe_list_destroy(TOEList * list);

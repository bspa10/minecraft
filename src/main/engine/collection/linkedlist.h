#pragma once

#include "list.h"

typedef struct {
    uint64_t allocated;

    void  * data;

    void  * next;
    void  * previews;
} TOELinkedNode;

void toe_linkedlist_clear(TOEList * list);
void toe_linkedlist_add(TOEList * list, void * data, uint64_t size);

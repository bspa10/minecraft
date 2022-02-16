#pragma once

#include "list.h"

struct TOELinkedNode{
    uint64_t allocated;

    void  * data;

    void  * next;
    void  * previews;
};

void toe_linkedlist_clear(struct TOEList * list);
void toe_linkedlist_add(struct TOEList * list, void * data, uint64_t size);

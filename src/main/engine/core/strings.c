#include <string.h>
#include "strings.h"
#include "../memory.h"

/**
 * Clone a string in a new memory region.
 *
 * @param original The original string to be cloned
 * @return Cloned string
 * @attention The resulting copy must be freed with <code> toe_memory_free(.., TOE_MEMORY_STRING) </code>
 */
string_t toe_str_clone(string_t original) {
    uint64_t length = strlen(original);

    string_t copy = toe_memory_allocate(length, TOE_MEMORY_STRING);
    toe_memory_copy(original, copy, length);

    return copy;
}

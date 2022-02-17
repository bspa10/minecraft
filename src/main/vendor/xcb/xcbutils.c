#include "xcbutils.h"

#include <stddef.h>
#include <string.h>

#include "../../engine/memory.h"
#include "../../engine/logger.h"
#include "../../engine/core/strings.h"

string_t get_xcb_error_code(uint8_t code) {
    switch (code) {
        default: return NULL;

        case  1: return toe_str_clone("1 (BadRequest)");
        case  2: return toe_str_clone("2 (BadValue)");
        case  3: return toe_str_clone("3 (BadWindow)");
        case  4: return toe_str_clone("4 (BadPixmap)");
        case  5: return toe_str_clone("5 (BadAtom)");
        case  6: return toe_str_clone("6 (BadCursor)");
        case  7: return toe_str_clone("7 (BadFont)");
        case  8: return toe_str_clone("8 (BadMatch)");
        case  9: return toe_str_clone("9 (BadDrawable)");
        case 10: return toe_str_clone("10 (BadAccess)");
        case 11: return toe_str_clone("11 (BadAlloc)");
        case 12: return toe_str_clone("12 (BadColormap)");
        case 13: return toe_str_clone("13 (BadGContext)");
        case 14: return toe_str_clone("14 (BadChoice)");
        case 15: return toe_str_clone("15 (BadName)");
        case 16: return toe_str_clone("16 (BadLength)");
        case 17: return toe_str_clone("17 (BadImplementation)");
    }

}

string_t get_xcb_major_code(uint8_t code) {
    switch (code) {
        default: return NULL;
        case  1: return toe_str_clone("1 (CreateWindow)");
        case  2: return toe_str_clone("2 (ChangeWindowAttributes)");
        case  3: return toe_str_clone("3 (GetWindowAttributes)");
        case  4: return toe_str_clone("4 (DestroyWindow)");
        case  5: return toe_str_clone("5 (DestroySubWindow)");
        case  6: return toe_str_clone("6 (ChangeSaveSet)");
        case  7: return toe_str_clone("7 (ReParentWindow)");
        case  8: return toe_str_clone("8 (MapWindow)");
        case  9: return toe_str_clone("9 (MapSubWindow)");
        case 10: return toe_str_clone("10 (UnMapWindow)");
        case 11: return toe_str_clone("11 (UnMapSubWindow)");
        case 12: return toe_str_clone("12 (ConfigureWindow)");
        case 13: return toe_str_clone("13 (CirculateWindow)");
        case 14: return toe_str_clone("14 (GetGeometry)");
        case 15: return toe_str_clone("15 (QueryTree)");
        case 16: return toe_str_clone("16 (InternAtom)");
        case 17: return toe_str_clone("17 (GetAtomName)");
        case 18: return toe_str_clone("18 (ChangeProperty)");
        case 19: return toe_str_clone("19 (DeleteProperty)");
        case 20: return toe_str_clone("20 (GetProperty)");
        case 40: return toe_str_clone("40 (TranslateCoords)");
    }
}

void toe_xcb_check_error(xcb_generic_error_t * error, cstring_t message) {
    if (error == NULL)
        return;

    string_t error_code = get_xcb_error_code(error->error_code);
    if (error_code == NULL) {
        TOE_LOG_FATAL(
                "%s :: XCB error: %d, sequence: %d, resource id: %d, major code: %d, minor code: %d",
                message,
                error->error_code,
                error->sequence,
                error->resource_id,
                error->major_code,
                error->minor_code
        )
    }

    string_t major_code = get_xcb_major_code(error->major_code);
    if (major_code != NULL) {
        TOE_LOG_FATAL(
                "%s :: XCB error: %s, sequence: %d, resource id: %d, major code: %s, minor code: %d",
                message,
                error_code,
                error->sequence,
                error->resource_id,
                major_code,
                error->minor_code
        )

    } else {
        TOE_LOG_FATAL(
                "%s :: XCB error: %s, sequence: %d, resource id: %d, major code: %d, minor code: %d",
                message,
                error_code,
                error->sequence,
                error->resource_id,
                error->major_code,
                error->minor_code
        )
    }
}

uint32_t toe_xcb_get_atom(xcb_connection_t * connection, cstring_t name, xcb_generic_error_t * error) {
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, 1, strlen(name), name);
    xcb_intern_atom_reply_t * reply = xcb_intern_atom_reply(connection, cookie, &error);
    toe_xcb_check_error(error, "xcb_intern_atom");

    uint32_t atom = reply->atom;
    toe_platform_memory_free(reply);

    return atom;
}

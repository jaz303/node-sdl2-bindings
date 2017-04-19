#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

METHOD(GetClipboardText) {
    BEGIN();
    RETURN(MK_STRING(SDL_GetClipboardText()));
}

METHOD(HasClipboardText) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasClipboardText()));
}

METHOD(SetClipboardText) {
    BEGIN();
    STRINGARG(text, 0);
    SDL_SetClipboardText(*text);
}

void InitClipboardFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "getClipboardText", GetClipboardText);
    NODE_SET_METHOD(exports, "hasClipboardText", HasClipboardText);
    NODE_SET_METHOD(exports, "setClipboardText", SetClipboardText);
}

}

#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

METHOD(ShowCursor) {
    BEGIN();
    INTARG(toggle, 0);
    int res = SDL_ShowCursor(toggle);
    if (res < 0) {
        THROW_SDL_ERROR();
    } else {
        RETURN(MK_NUMBER(res));
    }
}

void InitMouseFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "showCursor", ShowCursor);
}

}

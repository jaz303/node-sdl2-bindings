#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

METHOD(Init) {
    BEGIN();
    UINT32ARG(flags, 0);
    RETURN_INT(SDL_Init(flags));
}

METHOD(Delay) {
    BEGIN();
    UINT32ARG(ms, 0);
    SDL_Delay(ms);
}

METHOD(GetTicks) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetTicks()));
}

void InitCoreFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "init", Init);
    NODE_SET_METHOD(exports, "delay", Delay);
    NODE_SET_METHOD(exports, "getTicks", GetTicks);
}

}

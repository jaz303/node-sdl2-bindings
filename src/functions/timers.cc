#include "deps.h"

// Not implemented:
// SDL_AddTimer, SDL_RemoveTimer - uses threads
// SDL_GetPerformanceCounter, SDL_GetPerformanceFrequency - 64 bit values

namespace sdl2_bindings {

using namespace v8;

METHOD(Delay) {
    BEGIN();
    UINT32ARG(ms, 0);
    SDL_Delay(ms);
}

METHOD(GetTicks) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetTicks()));
}

METHOD(TicksPassed) {
    BEGIN();
    UINT32ARG(a, 0);
    UINT32ARG(b, 1);
    RETURN(MK_BOOL(SDL_TICKS_PASSED(a, b)));
}

void InitTimerFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "delay", Delay);
    NODE_SET_METHOD(exports, "getTicks", GetTicks);
    NODE_SET_METHOD(exports, "ticksPassed", TicksPassed);
}

}

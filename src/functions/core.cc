#include "deps.h"

// Not implemented:
// SDL_MainReady - do we need to worry about this?
// SDL_WinRTRunApp

namespace sdl2_bindings {

using namespace v8;

METHOD(Init) {
    BEGIN();
    UINT32ARG(flags, 0);
    if (SDL_Init(flags) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(InitSubSystem) {
    BEGIN();
    UINT32ARG(flags, 0);
    if (SDL_InitSubSystem(flags) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(Quit) {
    SDL_Quit();
}

METHOD(QuitSubSystem) {
    BEGIN();
    UINT32ARG(flags, 0);
    SDL_QuitSubSystem(flags);
}

METHOD(WasInit) {
    BEGIN();
    UINT32ARG(flags, 0);
    RETURN(MK_NUMBER(SDL_WasInit(flags)));
}

METHOD(GetPlatform) {
    BEGIN();
    RETURN(MK_STRING(SDL_GetPlatform()));
}

METHOD(GetVersion) {
    BEGIN();
    SDL_version linked;
    SDL_GetVersion(&linked);
    GET_CONTEXT();
    auto obj = MK_OBJECT();
    SET_KEY(obj, SYM(major), MK_NUMBER(linked.major));
    SET_KEY(obj, SYM(minor), MK_NUMBER(linked.minor));
    SET_KEY(obj, SYM(patch), MK_NUMBER(linked.patch));
    RETURN(obj);
}

METHOD(GetRevision) {
    BEGIN();
    RETURN(MK_STRING(SDL_GetRevision()));
}

METHOD(GetRevisionNumber) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetRevisionNumber()));
}

void InitCoreFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "init", Init);
    NODE_SET_METHOD(exports, "initSubSystem", InitSubSystem);
    NODE_SET_METHOD(exports, "quit", Quit);
    NODE_SET_METHOD(exports, "quitSubSystem", QuitSubSystem);
    NODE_SET_METHOD(exports, "wasInit", WasInit);
    NODE_SET_METHOD(exports, "getPlatform", GetPlatform);
    NODE_SET_METHOD(exports, "getVersion", GetVersion);
    NODE_SET_METHOD(exports, "getRevision", GetRevision);
    NODE_SET_METHOD(exports, "getRevisionNumber", GetRevisionNumber);
}

}

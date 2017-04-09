#include "deps.h"

// TODO: SDL_GL_ExtensionSupported
// TODO: SDL_GL_GetCurrentContext
// TODO: SDL_GL_GetCurrentWindow
// TODO: SDL_GL_GetDrawableSize
// TODO: SDL_GL_GetProcAddress
// TODO: SDL_GL_GetSwapInterval
// TODO: SDL_GL_LoadLibrary
// TODO: SDL_GL_SetSwapInterval
// TODO: SDL_GL_UnloadLibrary

namespace sdl2_bindings {

using namespace v8;

METHOD(GLCreateContext) {
    BEGIN();
    UNWRAP(window, Window, args[0]);
    auto ctx = SDL_GL_CreateContext(window->window_);
    if (ctx == nullptr) {
        THROW_SDL_ERROR();
    } else {
        RETURN(GLContext::NewInstance(isolate, ctx));
    }
}

METHOD(GLGetAttribute) {
    BEGIN();
    UINT32ARG(attr, 0);
    int value;
    if (SDL_GL_GetAttribute((SDL_GLattr)attr, &value) != 0) {
        THROW_SDL_ERROR();
    } else {
        RETURN(MK_NUMBER(value));
    }
}

METHOD(GLSetAttribute) {
    BEGIN();
    UINT32ARG(attr, 0);
    INTARG(value, 1);
    if (SDL_GL_SetAttribute((SDL_GLattr)attr, value) != 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(GLResetAttributes) {
    SDL_GL_ResetAttributes();
}

METHOD(GLMakeCurrent) {
    UNWRAP(window, Window, args[0]);
    UNWRAP(ctx, GLContext, args[1]);
    SDL_GL_MakeCurrent(window->window_, ctx->ctx_);
}

METHOD(GLSwapWindow) {
    UNWRAP(window, Window, args[0]);
    SDL_GL_SwapWindow(window->window_);
}

void InitGLFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "glCreateContext", GLCreateContext);
    NODE_SET_METHOD(exports, "glGetAttribute", GLGetAttribute);
    NODE_SET_METHOD(exports, "glSetAttribute", GLSetAttribute);
    NODE_SET_METHOD(exports, "glResetAttributes", GLResetAttributes);
    NODE_SET_METHOD(exports, "glMakeCurrent", GLMakeCurrent);
    NODE_SET_METHOD(exports, "glSwapWindow", GLSwapWindow);
}

}

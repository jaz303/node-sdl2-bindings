// SDL_GL_ExtensionSupported
// SDL_GL_GetCurrentContext
// SDL_GL_GetCurrentWindow
// SDL_GL_GetDrawableSize
// SDL_GL_GetProcAddress
// SDL_GL_GetSwapInterval
// SDL_GL_LoadLibrary
// SDL_GL_SetSwapInterval
// SDL_GL_UnloadLibrary

METHOD(GLGetAttribute) {
    BEGIN();
    UINT32ARG(attr, 0);
    int value;
    auto res = SDL_GL_GetAttribute((SDL_GLattr)attr, &value);
    if (res != 0) {
        THROW_SDL_ERROR();
    } else {
        RETURN(MK_NUMBER(value));
    }
}

METHOD(GLSetAttribute) {
    BEGIN();
    UINT32ARG(attr, 0);
    INTARG(value, 1);
    auto res = SDL_GL_SetAttribute((SDL_GLattr)attr, value);
    if (res != 0) {
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

void initGL(Local<Object> exports) {
    auto isolate = exports->GetIsolate();
    auto gl = MK_OBJECT();
    auto ctx = isolate->GetCurrentContext();
    exports->CreateDataProperty(ctx, SYM(gl), gl);
    NODE_SET_METHOD(gl, "getAttribute", GLGetAttribute);
    NODE_SET_METHOD(gl, "setAttribute", GLSetAttribute);
    NODE_SET_METHOD(gl, "resetAttributes", GLResetAttributes);
    NODE_SET_METHOD(gl, "makeCurrent", GLMakeCurrent);
    NODE_SET_METHOD(gl, "swapWindow", GLSwapWindow);
}
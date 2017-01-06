// TODO: SDL_GL_ExtensionSupported
// TODO: SDL_GL_GetCurrentContext
// TODO: SDL_GL_GetCurrentWindow
// TODO: SDL_GL_GetDrawableSize
// TODO: SDL_GL_GetProcAddress
// TODO: SDL_GL_GetSwapInterval
// TODO: SDL_GL_LoadLibrary
// TODO: SDL_GL_SetSwapInterval
// TODO: SDL_GL_UnloadLibrary

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
    NODE_SET_METHOD(gl, "createContext", GLCreateContext);
    NODE_SET_METHOD(gl, "getAttribute", GLGetAttribute);
    NODE_SET_METHOD(gl, "setAttribute", GLSetAttribute);
    NODE_SET_METHOD(gl, "resetAttributes", GLResetAttributes);
    NODE_SET_METHOD(gl, "makeCurrent", GLMakeCurrent);
    NODE_SET_METHOD(gl, "swapWindow", GLSwapWindow);
}
// TODO: GL stuff...
// SDL_GL_CreateContext
// SDL_GL_DeleteContext
// SDL_GL_ExtensionSupported
// SDL_GL_GetAttribute
// SDL_GL_GetCurrentContext
// SDL_GL_GetCurrentWindow
// SDL_GL_GetDrawableSize
// SDL_GL_GetProcAddress
// SDL_GL_GetSwapInterval
// SDL_GL_LoadLibrary
// SDL_GL_MakeCurrent
// SDL_GL_ResetAttributes
// SDL_GL_SetAttribute
// SDL_GL_SetSwapInterval
// SDL_GL_SwapWindow
// SDL_GL_UnloadLibrary

// TODO: SDL_GetWindowFromID - need internal map of ID => window object
// Might be easiest to implement the above in Javascript using a WeakMap?
// TODO: SDL_SetWindowHitTest - laziness
// TODO: SDL_SetWindowInputFocus - omitted
// TODO: SDL_SetWindowModalFor - omitted
// TODO: SDL_GetWindowWMInfo - omitted
// TODO: SDL_GetWindowGammaRamp - laziness
// TODO: SDL_SetWindowGammaRamp - laziness

v8::Persistent<v8::Function> Window::constructor;

Window::Window(SDL_Window *window) : window_(window) {}
Window::~Window() { destroy(); }

void Window::destroy() {
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
}

void Window::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Window"));
    tpl->InstanceTemplate()->SetInternalFieldCount(3);

    DEFINE_GETTER(tpl, "id", Id);
    DEFINE_GETTER(tpl, "surface", GetSurface);
    DEFINE_GETTER(tpl, "displayIndex", GetDisplayIndex);
    DEFINE_GETTER(tpl, "flags", GetFlags);
    
    DEFINE_METHOD(tpl, "update", Update);
    DEFINE_METHOD(tpl, "updatedRects", UpdateRects);
    DEFINE_METHOD(tpl, "createRenderer", CreateRenderer);
    DEFINE_METHOD(tpl, "hide", Hide);
    DEFINE_METHOD(tpl, "show", Show);
    DEFINE_METHOD(tpl, "maximize", Maximize);
    DEFINE_METHOD(tpl, "minimize", Minimize);
    DEFINE_METHOD(tpl, "raise", Raise);
    DEFINE_METHOD(tpl, "restore", Restore);
    DEFINE_METHOD(tpl, "showMessageBox", ShowMessageBox);
    DEFINE_METHOD(tpl, "showSimpleMessageBox", ShowSimpleMessageBox);
    DEFINE_METHOD(tpl, "getGrab", GetGrab);
    DEFINE_METHOD(tpl, "setGrab", SetGrab);
    DEFINE_METHOD(tpl, "setIcon", SetIcon);
    DEFINE_METHOD(tpl, "getMaximumSize", GetMaximumSize);
    DEFINE_METHOD(tpl, "getMinimumSize", GetMinimumSize);
    DEFINE_METHOD(tpl, "setMaximumSize", SetMaximumSize);
    DEFINE_METHOD(tpl, "setMinimumSize", SetMinimumSize);
    DEFINE_METHOD(tpl, "getDisplayMode", GetDisplayMode);
    DEFINE_METHOD(tpl, "getPixelFormat", GetPixelFormat);
    DEFINE_METHOD(tpl, "setBordered", SetBordered);
    DEFINE_METHOD(tpl, "setDisplayMode", SetDisplayMode);
    DEFINE_METHOD(tpl, "setFullscreen", SetFullscreen);
    DEFINE_METHOD(tpl, "getSize", GetSize);
    DEFINE_METHOD(tpl, "setSize", SetSize);
    DEFINE_METHOD(tpl, "getPosition", GetPosition);
    DEFINE_METHOD(tpl, "setPosition", SetPosition);
    DEFINE_METHOD(tpl, "getTitle", GetTitle);
    DEFINE_METHOD(tpl, "setTitle", SetTitle);
    DEFINE_METHOD(tpl, "getBrightness", GetBrightness);
    DEFINE_METHOD(tpl, "setBrightness", SetBrightness);

#ifdef SDL_GE_2_0_5
    DEFINE_METHOD(tpl, "getBordersSize", GetBordersSize);
    DEFINE_METHOD(tpl, "setResizable", SetResizable);
    DEFINE_METHOD(tpl, "getOpacity", GetOpacity);
    DEFINE_METHOD(tpl, "setOpacity", SetOpacity);
#endif

    constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> Window::NewInstance(Isolate *isolate, SDL_Window *window) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    Window *w = new Window(window);
    w->Wrap(instance);
    return instance;
}

GETTER(Window::Id) {
    BEGIN();
    UNWRAP_ME(self, Window);
    RETURN(MK_NUMBER(SDL_GetWindowID(self->window_)));
}

GETTER(Window::GetSurface) {
    BEGIN();
    auto theSurface = args.This()->GetInternalField(1);
    if (theSurface->IsUndefined()) {
        UNWRAP_ME(w, Window);
        theSurface = Surface::NewInstance(isolate, SDL_GetWindowSurface(w->window_), false);
        args.This()->SetInternalField(1, theSurface);
    }
    RETURN(theSurface);
}

GETTER(Window::GetDisplayIndex) {
    BEGIN();
    UNWRAP_ME(self, Window);
    RETURN(MK_NUMBER(SDL_GetWindowDisplayIndex(self->window_)));
}

GETTER(Window::GetFlags) {
    BEGIN();
    UNWRAP_ME(self, Window);
    RETURN(MK_NUMBER(SDL_GetWindowFlags(self->window_)));
}

METHOD(Window::New) {}

METHOD(Window::Update) {
    UNWRAP_ME(w, Window);
    SDL_UpdateWindowSurface(w->window_);
}

METHOD(Window::UpdateRects) {
    BEGIN();
    ARRAYARG(rects, 0);
    uint32_t numRects = rects->Length();
    SDL_Rect sdlRects[numRects];
    for (uint32_t ix = 0; ix < numRects; ++ix) {
        extractRect(isolate, rects->Get(ix)->ToObject(), &sdlRects[ix]);
    }
    UNWRAP_ME(self, Window);
    if (SDL_UpdateWindowSurfaceRects(self->window_, sdlRects, numRects) != 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(Window::CreateRenderer) {
    BEGIN();
    int index = -1;
    uint32_t flags = 0;
    if (args.Length() == 2) {
        index = args[0]->Int32Value();
        flags = args[1]->Uint32Value();
    }
    UNWRAP_ME(w, Window);
    SDL_Renderer *renderer = SDL_CreateRenderer(w->window_, index, flags);
    if (!renderer) {
        THROW(Error, "could not create renderer");
    }
    RETURN(Renderer::NewInstance(isolate, renderer));
}

METHOD(Window::Hide) {
    BEGIN();
    UNWRAP_ME(self, Window);
    SDL_HideWindow(self->window_);
}

METHOD(Window::Show) {
    BEGIN();
    UNWRAP_ME(self, Window);
    SDL_ShowWindow(self->window_);
}

METHOD(Window::Maximize) {
    BEGIN();
    UNWRAP_ME(self, Window);
    SDL_MaximizeWindow(self->window_);
}

METHOD(Window::Minimize) {
    BEGIN();
    UNWRAP_ME(self, Window);
    SDL_MinimizeWindow(self->window_);
}

METHOD(Window::Raise) {
    BEGIN();
    UNWRAP_ME(self, Window);
    SDL_RaiseWindow(self->window_);
}

METHOD(Window::Restore) {
    BEGIN();
    UNWRAP_ME(self, Window);
    SDL_RestoreWindow(self->window_);
}

METHOD(Window::ShowMessageBox) {
    BEGIN();
    UNWRAP_ME(self, Window);
    showMessageBox(args, self->window_);
}

METHOD(Window::ShowSimpleMessageBox) {
    BEGIN();
    UNWRAP_ME(self, Window);
    showSimpleMessageBox(args, self->window_);
}

METHOD(Window::GetGrab) {
    BEGIN();
    UNWRAP_ME(self, Window);
    RETURN_BOOL(SDL_GetWindowGrab(self->window_) == SDL_TRUE);
}

METHOD(Window::SetGrab) {
    BEGIN();
    NARGS(1);
    UNWRAP_ME(self, Window);
    SDL_SetWindowGrab(self->window_, args[0]->BooleanValue() ? SDL_TRUE : SDL_FALSE);
}

METHOD(Window::SetIcon) {
    BEGIN();
    NARGS(1);
    UNWRAP_ME(self, Window);
    UNWRAP(icon, Surface, args[0]);
    SDL_SetWindowIcon(self->window_, icon->surface_);
}

METHOD(Window::GetMaximumSize) {
    BEGIN();
    UNWRAP_ME(self, Window);
    int w, h;
    SDL_GetWindowMaximumSize(self->window_, &w, &h);
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(width), MK_NUMBER(w));
    SET_KEY(out, SYM(width), MK_NUMBER(h));
    RETURN(out);
}

METHOD(Window::GetMinimumSize) {
    BEGIN();
    UNWRAP_ME(self, Window);
    int w, h;
    SDL_GetWindowMinimumSize(self->window_, &w, &h);
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(width), MK_NUMBER(w));
    SET_KEY(out, SYM(width), MK_NUMBER(h));
    RETURN(out);
}

METHOD(Window::SetMaximumSize) {
    BEGIN();
    NARGS(2);
    INTARG(w, 0);
    INTARG(h, 1);
    UNWRAP_ME(self, Window);
    SDL_SetWindowMaximumSize(self->window_, w, h);
}

METHOD(Window::SetMinimumSize) {
    BEGIN();
    NARGS(2);
    INTARG(w, 0);
    INTARG(h, 1);
    UNWRAP_ME(self, Window);
    SDL_SetWindowMinimumSize(self->window_, w, h);
}

METHOD(Window::GetDisplayMode) {
    BEGIN();
    UNWRAP_ME(self, Window);
    SDL_DisplayMode displayMode;
    if (SDL_GetWindowDisplayMode(self->window_, &displayMode) != 0) {
        THROW_SDL_ERROR();
    }
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(format), MK_NUMBER(displayMode.format));
    SET_KEY(out, SYM(width), MK_NUMBER(displayMode.w));
    SET_KEY(out, SYM(height), MK_NUMBER(displayMode.h));
    SET_KEY(out, SYM(refreshRate), MK_NUMBER(displayMode.refresh_rate));
    RETURN(out);
}

METHOD(Window::GetPixelFormat) {
    BEGIN();
    UNWRAP_ME(self, Window);
    RETURN(MK_NUMBER(SDL_GetWindowPixelFormat(self->window_)));
}

METHOD(Window::SetFullscreen) {
    BEGIN();
    NARGS(1);
    UINT32ARG(flags, 0);
    UNWRAP_ME(self, Window);
    if (SDL_SetWindowFullscreen(self->window_, flags) != 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(Window::SetBordered) {
    BEGIN();
    NARGS(1);
    UNWRAP_ME(self, Window);
    SDL_SetWindowBordered(self->window_, args[0]->BooleanValue() ? SDL_TRUE : SDL_FALSE);
}

METHOD(Window::SetDisplayMode) {
    BEGIN();
    NARGS(1);
    OBJECTARG(displayMode, 0);
    UNWRAP_ME(self, Window);
    SDL_DisplayMode sdlDisplayMode;
    extractDisplayMode(isolate, displayMode, &sdlDisplayMode);
    if (SDL_SetWindowDisplayMode(self->window_, &sdlDisplayMode) != 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(Window::GetSize) {
    BEGIN();
    UNWRAP_ME(self, Window);
    int w, h;
    SDL_GetWindowSize(self->window_, &w, &h);
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(width), MK_NUMBER(w));
    SET_KEY(out, SYM(height), MK_NUMBER(h));
    RETURN(out);
}

METHOD(Window::SetSize) {
    BEGIN();
    NARGS(2);
    INTARG(width, 0);
    INTARG(height, 1);
    UNWRAP_ME(self, Window);
    SDL_SetWindowSize(self->window_, width, height);
}

METHOD(Window::GetPosition) {
    BEGIN();
    UNWRAP_ME(self, Window);
    int x, y;
    SDL_GetWindowPosition(self->window_, &x, &y);
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(x), MK_NUMBER(x));
    SET_KEY(out, SYM(y), MK_NUMBER(y));
    RETURN(out);
}

METHOD(Window::SetPosition) {
    BEGIN();
    NARGS(2);
    INTARG(x, 0);
    INTARG(y, 1);
    UNWRAP_ME(self, Window);
    SDL_SetWindowPosition(self->window_, x, y);    
}

METHOD(Window::GetTitle) {
    BEGIN();
    UNWRAP_ME(w, Window);
    RETURN(MK_STRING(SDL_GetWindowTitle(w->window_)));
}

METHOD(Window::SetTitle) {
    BEGIN();
    NARGS(1);
    STRINGARG(title, 0);
    UNWRAP_ME(w, Window);
    SDL_SetWindowTitle(w->window_, *title);
}

METHOD(Window::GetBrightness) {
    BEGIN();
    UNWRAP_ME(self, Window);
    RETURN(MK_NUMBER(SDL_GetWindowBrightness(self->window_)));
}

METHOD(Window::SetBrightness) {
    BEGIN();
    NARGS(1);
    DOUBLEARG(brightness, 0);
    UNWRAP_ME(self, Window);
    SDL_SetWindowBrightness(self->window_, brightness);
}

#ifdef SDL_GE_2_0_5

METHOD(Window::GetBordersSize) {
    BEGIN();
    UNWRAP_ME(self, Window);
    int top, left, bottom, right;
    SDL_GetWindowBordersSize(self->window_, &top, &left, &bottom, &right);
    auto out = v8::Object::New(isolate);
    GET_CONTEXT();
    SET_KEY(out, SYM(top), MK_NUMBER(top));
    SET_KEY(out, SYM(left), MK_NUMBER(left));
    SET_KEY(out, SYM(bottom), MK_NUMBER(bottom));
    SET_KEY(out, SYM(right), MK_NUMBER(right));
    RETURN(out);
}

METHOD(Window::SetResizable) {
    BEGIN();
    NARGS(1);
    UNWRAP_ME(self, Window);
    SDL_SetWindowResizable(self->window_, args[0]->BooleanValue() ? SDL_TRUE : SDL_FALSE);
}

METHOD(Window::GetOpacity) {
    BEGIN();
    UNWRAP_ME(self, Window);
    float out;
    if (SDL_GetWindowOpacity(self->window_, &out) == 0) {
        RETURN(MK_NUMBER(out));
    } else {
        THROW_SDL_ERROR();
    }
}

METHOD(Window::SetOpacity) {
    BEGIN();
    NARGS(1);
    DOUBLEARG(opacity, 0);
    UNWRAP_ME(self, Window);
    if (SDL_SetWindowOpacity(self->window_, opacity != 0)) {
        THROW_SDL_ERROR();
    }
}

#endif
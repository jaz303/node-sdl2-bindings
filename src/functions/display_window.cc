#include "deps.h"

// Not implemented:
// SDL_CreateWindowAndRenderer - just use two separate calls
// SDL_CreateWindowFrom - tricky, not particularly useful
// SDL_GetWindowData, SDL_SetWindowData - just add properties to JS window object
// SDL_SetWindowModalFor - omitted as X11 only
// SDL_GetWindowWMInfo - omitted, limited use

// To be implemented:
// TODO: SDL_SetWindowHitTest - laziness
// TODO: SDL_GetWindowGammaRamp - laziness
// TODO: SDL_SetWindowGammaRamp - laziness
// TODO: SDL_ShowMessageBox - laziness

namespace sdl2_bindings {

using namespace v8;

METHOD(GetClosestDisplayMode) {
    BEGIN();
    INTARG(index, 0);
    OBJECTARG(ref, 1);
    SDL_DisplayMode referenceMode, closestMode;
    extractDisplayMode(isolate, ref, &referenceMode);
    if (SDL_GetClosestDisplayMode(index, &referenceMode, &closestMode) == nullptr) {
        THROW_SDL_ERROR();
    } else {
        auto obj = MK_OBJECT();
        populateDisplayMode(isolate, obj, &closestMode);
        RETURN(obj);
    }
}

METHOD(GetCurrentDisplayMode) {
    BEGIN();
    INTARG(index, 0);
    SDL_DisplayMode mode;
    if (SDL_GetCurrentDisplayMode(index, &mode) < 0) {
        THROW_SDL_ERROR();
    } else {
        auto obj = MK_OBJECT();
        populateDisplayMode(isolate, obj, &mode);
        RETURN(obj);   
    }
}

METHOD(GetDesktopDisplayMode) {
    BEGIN();
    INTARG(index, 0);
    SDL_DisplayMode mode;
    if (SDL_GetDesktopDisplayMode(index, &mode) < 0) {
        THROW_SDL_ERROR();
    } else {
        auto obj = MK_OBJECT();
        populateDisplayMode(isolate, obj, &mode);
        RETURN(obj);   
    }
}

METHOD(GetVideoDriver) {
    BEGIN();
    INTARG(index, 0);
    RETURN(MK_STRING(SDL_GetVideoDriver(index)));
}

METHOD(GetCurrentVideoDriver) {
    BEGIN();
    RETURN(MK_STRING(SDL_GetCurrentVideoDriver()));
}

METHOD(GetNumDisplayModes) {
    BEGIN();
    INTARG(index, 0);
    RETURN(MK_NUMBER(SDL_GetNumDisplayModes(index)));
}

METHOD(GetNumVideoDisplays) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetNumVideoDisplays()));
}

METHOD(GetNumVideoDrivers) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetNumVideoDrivers()));
}

METHOD(GetDisplayBounds) {
    BEGIN();
    INTARG(index, 0);
    SDL_Rect rect;
    if (SDL_GetDisplayBounds(index, &rect) < 0) {
        THROW_SDL_ERROR();
    } else {
        auto out = MK_OBJECT();
        populateRect(isolate, out, &rect);
        RETURN(out);
    }
}

METHOD(GetDisplayDPI) {
    BEGIN();
    INTARG(index, 0);
    float diagonal, horizontal, vertical;
    if (SDL_GetDisplayDPI(index, &diagonal, &horizontal, &vertical) < 0) {
        THROW_SDL_ERROR();
    } else {
        auto out = MK_OBJECT();
        GET_CONTEXT();
        SET_KEY(out, SYM(diagonal), MK_NUMBER(diagonal));
        SET_KEY(out, SYM(horizontal), MK_NUMBER(horizontal));
        SET_KEY(out, SYM(vertical), MK_NUMBER(vertical));
        RETURN(out);
    }
}

METHOD(GetDisplayMode) {
    BEGIN();
    INTARG(dIndex, 0);
    INTARG(mIndex, 1);
    SDL_DisplayMode displayMode;
    if (SDL_GetDisplayMode(dIndex, mIndex, &displayMode) < 0) {
        THROW_SDL_ERROR();
    } else {
        auto out = MK_OBJECT();
        populateDisplayMode(isolate, out, &displayMode);
        RETURN(out);
    }
}

METHOD(GetDisplayName) {
    BEGIN();
    INTARG(index, 0);
    auto res = SDL_GetDisplayName(index);
    if (res == nullptr) {
        THROW_SDL_ERROR();
    } else {
        RETURN(MK_STRING(res));    
    }
}

METHOD(GetDisplayUsableBounds) {
    BEGIN();
    INTARG(index, 0);
    SDL_Rect rect;
    if (SDL_GetDisplayUsableBounds(index, &rect) < 0) {
        THROW_SDL_ERROR();
    } else {
        auto out = MK_OBJECT();
        populateRect(isolate, out, &rect);
        RETURN(out);
    }
}

METHOD(GetWindowFromId) {
    BEGIN();
    UINT32ARG(id, 0);
    SDL_Window *window = SDL_GetWindowFromID(id);
    if (window == nullptr) {
        THROW_SDL_ERROR();
    } else {
        auto wrappedWindow = getWindowReference(window);
    }
}

METHOD(GetGrabbedWindow) {
    BEGIN();
    SDL_Window *window = SDL_GetGrabbedWindow();
    if (window == nullptr) {
        RETURN(MK_NULL());
    } else {
        auto wrappedWindow = getWindowReference(window);
    }
}

METHOD(CreateWindow) {
	BEGIN();
	STRINGARG(title, 0);
	INTARG(x, 1);
	INTARG(y, 2);
	INTARG(w, 3);
	INTARG(h, 4);
	UINT32ARG(flags, 5);
	SDL_Window *win = SDL_CreateWindow(*title, x, y, w, h, flags);
	if (!win) {
		THROW_SDL_ERROR();
	} else {
		RETURN(Window::NewInstance(isolate, win));
	}
}

METHOD(DisableScreenSaver) {
    SDL_DisableScreenSaver();
}

METHOD(EnableScreenSaver) {
    SDL_EnableScreenSaver();   
}

METHOD(IsScreenSaverEnabled) {
    BEGIN();
    RETURN(MK_BOOL(SDL_IsScreenSaverEnabled()));
}

METHOD(HideWindow) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    SDL_HideWindow(self->window_);
}

METHOD(ShowWindow) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    SDL_ShowWindow(self->window_);
}

METHOD(MaximizeWindow) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    SDL_MaximizeWindow(self->window_);
}

METHOD(MinimizeWindow) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    SDL_MinimizeWindow(self->window_);
}

METHOD(RaiseWindow) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    SDL_RaiseWindow(self->window_);   
}

METHOD(RestoreWindow) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    SDL_RestoreWindow(self->window_);   
}

METHOD(GetWindowID) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    RETURN(MK_NUMBER(SDL_GetWindowID(self->window_)));
}

METHOD(GetWindowSurface) {
    BEGIN();
    OBJECTARG(window, 0);
    UNWRAP(self, Window, window);
    auto theSurface = window->GetInternalField(1);
    if (theSurface->IsUndefined()) {
        theSurface = Surface::NewInstance(isolate, SDL_GetWindowSurface(self->window_), false);
        window->SetInternalField(1, theSurface);
    }
    RETURN(theSurface);
}

METHOD(GetWindowDisplayIndex) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    RETURN(MK_NUMBER(SDL_GetWindowDisplayIndex(self->window_)));
}

METHOD(GetWindowFlags) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    RETURN(MK_NUMBER(SDL_GetWindowFlags(self->window_)));
}

METHOD(UpdateWindowSurface) {
    UNWRAP(self, Window, args[0]);
    SDL_UpdateWindowSurface(self->window_);
}

METHOD(UpdateWindowSurfaceRects) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    ARRAYARG(rects, 1);
    uint32_t numRects = rects->Length();
    SDL_Rect sdlRects[numRects];
    for (uint32_t ix = 0; ix < numRects; ++ix) {
        extractRect(isolate, rects->Get(ix)->ToObject(), &sdlRects[ix]);
    }
    if (SDL_UpdateWindowSurfaceRects(self->window_, sdlRects, numRects) != 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(ShowSimpleMessageBox) {
    BEGIN();
    STRINGARG(title, 0);
    STRINGARG(message, 1);
    UINT32ARG(flags, 2);
    SDL_Window *windowPtr = nullptr;
    if (!args[3]->IsNull()) {
        UNWRAP(window, Window, args[3]);
        windowPtr = window->window_;    
    }
    if (SDL_ShowSimpleMessageBox(flags, *title, *message, windowPtr) != 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(GetWindowGrab) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    RETURN_BOOL(SDL_GetWindowGrab(self->window_) == SDL_TRUE);
}

METHOD(SetWindowGrab) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    SDL_SetWindowGrab(self->window_, args[1]->BooleanValue() ? SDL_TRUE : SDL_FALSE);
}

METHOD(SetWindowIcon) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    UNWRAP(icon, Surface, args[1]);
    SDL_SetWindowIcon(self->window_, icon->surface_);
}

METHOD(GetWindowMaximumSize) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    int w, h;
    SDL_GetWindowMaximumSize(self->window_, &w, &h);
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(width), MK_NUMBER(w));
    SET_KEY(out, SYM(width), MK_NUMBER(h));
    RETURN(out);
}

METHOD(GetWindowMinimumSize) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    int w, h;
    SDL_GetWindowMinimumSize(self->window_, &w, &h);
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(width), MK_NUMBER(w));
    SET_KEY(out, SYM(width), MK_NUMBER(h));
    RETURN(out);
}

METHOD(SetWindowMaximumSize) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    INTARG(w, 1);
    INTARG(h, 2);
    SDL_SetWindowMaximumSize(self->window_, w, h);
}

METHOD(SetWindowMinimumSize) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    INTARG(w, 1);
    INTARG(h, 2);
    SDL_SetWindowMinimumSize(self->window_, w, h);
}

METHOD(GetWindowPixelFormat) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    RETURN(MK_NUMBER(SDL_GetWindowPixelFormat(self->window_)));
}

METHOD(SetWindowFullscreen) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    UINT32ARG(flags, 1);
    if (SDL_SetWindowFullscreen(self->window_, flags) != 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(SetWindowBordered) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    SDL_SetWindowBordered(self->window_, args[1]->BooleanValue() ? SDL_TRUE : SDL_FALSE);
}

METHOD(GetWindowDisplayMode) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    SDL_DisplayMode displayMode;
    if (SDL_GetWindowDisplayMode(self->window_, &displayMode) != 0) {
        THROW_SDL_ERROR();
    }
    auto out = MK_OBJECT();
    populateDisplayMode(isolate, out, &displayMode);
    RETURN(out);
}

METHOD(SetWindowDisplayMode) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    OBJECTARG(displayMode, 1);
    SDL_DisplayMode sdlDisplayMode;
    extractDisplayMode(isolate, displayMode, &sdlDisplayMode);
    if (SDL_SetWindowDisplayMode(self->window_, &sdlDisplayMode) != 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(GetWindowSize) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    int w, h;
    SDL_GetWindowSize(self->window_, &w, &h);
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(width), MK_NUMBER(w));
    SET_KEY(out, SYM(height), MK_NUMBER(h));
    RETURN(out);
}

METHOD(SetWindowSize) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    INTARG(width, 1);
    INTARG(height, 2);
    SDL_SetWindowSize(self->window_, width, height);
}

METHOD(GetWindowPosition) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    int x, y;
    SDL_GetWindowPosition(self->window_, &x, &y);
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(x), MK_NUMBER(x));
    SET_KEY(out, SYM(y), MK_NUMBER(y));
    RETURN(out);
}

METHOD(SetWindowPosition) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    INTARG(x, 1);
    INTARG(y, 2);
    SDL_SetWindowPosition(self->window_, x, y);    
}

METHOD(GetWindowTitle) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    RETURN(MK_STRING(SDL_GetWindowTitle(self->window_)));
}

METHOD(SetWindowTitle) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    STRINGARG(title, 1);
    SDL_SetWindowTitle(self->window_, *title);
}

METHOD(GetWindowBrightness) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    RETURN(MK_NUMBER(SDL_GetWindowBrightness(self->window_)));
}

METHOD(SetWindowBrightness) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    DOUBLEARG(brightness, 1);
    SDL_SetWindowBrightness(self->window_, brightness);
}

METHOD(SetWindowInputFocus) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    if (SDL_SetWindowInputFocus(self->window_) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(VideoInit) {
    BEGIN();
    int res;
    if (args[0]->IsNull()) {
        res = SDL_VideoInit(nullptr);
    } else {
        String::Utf8Value driverName(args[0]);
        res = SDL_VideoInit(*driverName);
    }
    if (res < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(VideoQuit) {
    SDL_VideoQuit();
}

#if SDL_VERSION_ATLEAST(2, 0, 5)

METHOD(GetWindowBordersSize) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
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

METHOD(SetWindowResizable) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    SDL_SetWindowResizable(self->window_, args[0]->BooleanValue() ? SDL_TRUE : SDL_FALSE);
}

METHOD(GetWindowOpacity) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    float out;
    if (SDL_GetWindowOpacity(self->window_, &out) == 0) {
        RETURN(MK_NUMBER(out));
    } else {
        THROW_SDL_ERROR();
    }
}

METHOD(SetWindowOpacity) {
    BEGIN();
    UNWRAP(self, Window, args[0]);
    DOUBLEARG(opacity, 1);
    if (SDL_SetWindowOpacity(self->window_, opacity != 0)) {
        THROW_SDL_ERROR();
    }
}

#endif

void InitDisplayWindowFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "getClosestDisplayMode", GetClosestDisplayMode);
    NODE_SET_METHOD(exports, "getCurrentDisplayMode", GetCurrentDisplayMode);
    NODE_SET_METHOD(exports, "getDesktopDisplayMode", GetDesktopDisplayMode);
    NODE_SET_METHOD(exports, "getVideoDriver", GetVideoDriver);
    NODE_SET_METHOD(exports, "getCurrentVideoDriver", GetCurrentVideoDriver);
    NODE_SET_METHOD(exports, "getNumDisplayModes", GetNumDisplayModes);
    NODE_SET_METHOD(exports, "getNumVideoDisplays", GetNumVideoDisplays);
    NODE_SET_METHOD(exports, "getNumVideoDrivers", GetNumVideoDrivers);
    NODE_SET_METHOD(exports, "getDisplayBounds", GetDisplayBounds);
    NODE_SET_METHOD(exports, "getDisplayDPI", GetDisplayDPI);
    NODE_SET_METHOD(exports, "getDisplayMode", GetDisplayMode);
    NODE_SET_METHOD(exports, "getDisplayName", GetDisplayName);
    NODE_SET_METHOD(exports, "getDisplayUsableBounds", GetDisplayUsableBounds);

	NODE_SET_METHOD(exports, "createWindow", CreateWindow);

    NODE_SET_METHOD(exports, "disableScreenSaver", DisableScreenSaver);
    NODE_SET_METHOD(exports, "enableScreenSaver", EnableScreenSaver);
    NODE_SET_METHOD(exports, "isScreenSaverEnabled", IsScreenSaverEnabled);

    NODE_SET_METHOD(exports, "hideWindow", HideWindow);
    NODE_SET_METHOD(exports, "showWindow", ShowWindow);
    NODE_SET_METHOD(exports, "minimizeWindow", MinimizeWindow);
    NODE_SET_METHOD(exports, "maximizeWindow", MaximizeWindow);
    NODE_SET_METHOD(exports, "raiseWindow", RaiseWindow);
    NODE_SET_METHOD(exports, "restoreWindow", RestoreWindow);

    NODE_SET_METHOD(exports, "getWindowId", GetWindowID);
    NODE_SET_METHOD(exports, "getWindowSurface", GetWindowSurface);
    NODE_SET_METHOD(exports, "getWindowDisplayIndex", GetWindowDisplayIndex);
    NODE_SET_METHOD(exports, "getWindowFlags", GetWindowFlags);

    NODE_SET_METHOD(exports, "updateWindowSurface", UpdateWindowSurface);
    NODE_SET_METHOD(exports, "updateWindowSurfaceRects", UpdateWindowSurfaceRects);

    NODE_SET_METHOD(exports, "showSimpleMessageBox", ShowSimpleMessageBox);

    NODE_SET_METHOD(exports, "getWindowGrab", GetWindowGrab);
    NODE_SET_METHOD(exports, "setWindowGrab", SetWindowGrab);

    NODE_SET_METHOD(exports, "setWindowIcon", SetWindowIcon);

    NODE_SET_METHOD(exports, "getWindowPixelFormat", GetWindowPixelFormat);

    NODE_SET_METHOD(exports, "setWindowFullscreen", SetWindowFullscreen);
    NODE_SET_METHOD(exports, "setWindowBordered", SetWindowBordered);
    
    NODE_SET_METHOD(exports, "getWindowDisplayMode", GetWindowDisplayMode);
    NODE_SET_METHOD(exports, "setWindowDisplayMode", SetWindowDisplayMode);

    NODE_SET_METHOD(exports, "getWindowFromId", GetWindowFromId);
    NODE_SET_METHOD(exports, "getGrabbedWindow", GetGrabbedWindow);

    NODE_SET_METHOD(exports, "getWindowSize", GetWindowSize);
    NODE_SET_METHOD(exports, "setWindowSize", SetWindowSize);
    NODE_SET_METHOD(exports, "getWindowMaximumSize", GetWindowMaximumSize);
    NODE_SET_METHOD(exports, "setWindowMaximumSize", SetWindowMaximumSize);
    NODE_SET_METHOD(exports, "getWindowMinimumSize", GetWindowMinimumSize);
    NODE_SET_METHOD(exports, "setWindowMinimumSize", SetWindowMinimumSize);

    NODE_SET_METHOD(exports, "getWindowPosition", GetWindowPosition);
    NODE_SET_METHOD(exports, "setWindowPosition", SetWindowPosition);

    NODE_SET_METHOD(exports, "getWindowTitle", GetWindowTitle);
    NODE_SET_METHOD(exports, "setWindowTitle", SetWindowTitle);

    NODE_SET_METHOD(exports, "getWindowBrightness", GetWindowBrightness);
    NODE_SET_METHOD(exports, "setWindowBrightness", SetWindowBrightness);

    NODE_SET_METHOD(exports, "setWindowInputFocus", SetWindowInputFocus);

    NODE_SET_METHOD(exports, "videoInit", VideoInit);
    NODE_SET_METHOD(exports, "videoQuit", VideoQuit);

#if SDL_VERSION_ATLEAST(2, 0, 5)
    NODE_SET_METHOD(exports, "getWindowBordersSize", GetWindowBordersSize);
    NODE_SET_METHOD(exports, "setWindowResizable", SetWindowResizable);
    NODE_SET_METHOD(exports, "getWindowOpacity", GetWindowOpacity);
    NODE_SET_METHOD(exports, "setWindowOpacity", SetWindowOpacity);
#endif

}

}

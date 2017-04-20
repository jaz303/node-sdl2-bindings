#include "deps.h"

// Not implemented:
// SDL_GetKeyboardFocus() - need way of mapping SDL_Window* to JS Window

namespace sdl2_bindings {

using namespace v8;

METHOD(GetKeyFromName) {
    BEGIN();
    STRINGARG(name, 0);
    auto key = SDL_GetKeyFromName(*name);
    if (key == SDLK_UNKNOWN) {
        THROW_SDL_ERROR();
    } else {
        RETURN(MK_NUMBER(key));
    }
}

METHOD(GetKeyFromScancode) {
    BEGIN();
    INTARG(scancode, 0);
    auto key = SDL_GetKeyFromScancode((SDL_Scancode)scancode);
    RETURN(MK_NUMBER(key));
}

METHOD(GetKeyName) {
    BEGIN();
    INTARG(key, 0);
    RETURN(MK_STRING(SDL_GetKeyName(key)));
}

METHOD(GetKeyboardState) {
    BEGIN();
    int len;
    const Uint8 *state  = SDL_GetKeyboardState(&len);
    Local<Array> out = Array::New(isolate, len);
    for (int ix = 0; ix < len; ++ix) {
        out->Set(ix, MK_BOOL(state[ix] == 1));
    }
    RETURN(out);
}

METHOD(ReadKeyboardState) {
    BEGIN();
    ARRAYARG(target, 0);
    int len;
    const Uint8 *state  = SDL_GetKeyboardState(&len);
    for (int ix = 0; ix < len; ++ix) {
        target->Set(ix, MK_BOOL(state[ix] == 1));
    }
}

METHOD(GetModState) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetModState()));
}

METHOD(GetScancodeFromKey) {
    BEGIN();
    INTARG(key, 0);
    RETURN(MK_NUMBER(SDL_GetScancodeFromKey(key)));
}

METHOD(GetScancodeFromName) {
    BEGIN();
    STRINGARG(name, 0);
    auto sc = SDL_GetScancodeFromName(*name);
    if (sc == SDL_SCANCODE_UNKNOWN) {
        THROW_SDL_ERROR();
    } else {
        RETURN(MK_NUMBER(sc));
    }
}

METHOD(GetScancodeName) {
    BEGIN();
    INTARG(sc, 0);
    RETURN(MK_STRING(SDL_GetScancodeName((SDL_Scancode)sc)));
}

METHOD(HasScreenKeyboardSupport) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasScreenKeyboardSupport() == SDL_TRUE));
}

METHOD(IsScreenKeyboardShown) {
    BEGIN();
    UNWRAP(window, Window, args[0]);
    RETURN(MK_BOOL(SDL_IsScreenKeyboardShown(window->window_) == SDL_TRUE));
}

METHOD(IsTextInputActive) {
    BEGIN();
    RETURN(MK_BOOL(SDL_IsTextInputActive() == SDL_TRUE));
}

METHOD(SetModState) {
    BEGIN();
    INTARG(state, 0);
    SDL_SetModState((SDL_Keymod)state);
}

METHOD(SetTextInputRect) {
    BEGIN();
    SDL_Rect rect;
    SDL_Rect *rectptr = nullptr;
    if (!args[0]->IsNull()) {
        OBJECTARG(rectObj, 0);
        extractRect(isolate, rectObj, &rect);
        rectptr = &rect;
    }
    SDL_SetTextInputRect(rectptr);
}

METHOD(StartTextInput) {
    SDL_StartTextInput();
}

METHOD(StopTextInput) {
    SDL_StopTextInput();
}

void InitKeyboardFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "getKeyFromName", GetKeyFromName);
    NODE_SET_METHOD(exports, "getKeyFromScancode", GetKeyFromScancode);
    NODE_SET_METHOD(exports, "getKeyName", GetKeyName);
    NODE_SET_METHOD(exports, "getKeyboardState", GetKeyboardState);
    NODE_SET_METHOD(exports, "readKeyboardState", ReadKeyboardState);
    NODE_SET_METHOD(exports, "getModState", GetModState);
    NODE_SET_METHOD(exports, "getScancodeFromKey", GetScancodeFromKey);
    NODE_SET_METHOD(exports, "getScancodeFromName", GetScancodeFromName);
    NODE_SET_METHOD(exports, "getScancodeName", GetScancodeName);
    NODE_SET_METHOD(exports, "hasScreenKeyboardSupport", HasScreenKeyboardSupport);
    NODE_SET_METHOD(exports, "isScreenKeyboardShown", IsScreenKeyboardShown);
    NODE_SET_METHOD(exports, "isTextInputActive", IsTextInputActive);
    NODE_SET_METHOD(exports, "setModState", SetModState);
    NODE_SET_METHOD(exports, "setTextInputRect", SetTextInputRect);
    NODE_SET_METHOD(exports, "startTextInput", StartTextInput);
    NODE_SET_METHOD(exports, "stopTextInput", StopTextInput);
}

}

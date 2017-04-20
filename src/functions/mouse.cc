#include "deps.h"

// TODO:
// SDL_GetMouseFocus() - need a way to map window pointers to Window objects
//
// Play about with GetCursor/GetDefaultCursor - if they're stable we should 
// introduce a Javascript shim to return the same instance.

namespace sdl2_bindings {

using namespace v8;

METHOD(CreateColorCursor) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	INTARG(x, 1);
	INTARG(y, 2);
	SDL_Cursor *cursor = SDL_CreateColorCursor(surface->surface_, x, y);
	if (cursor == nullptr) {
		THROW_SDL_ERROR();
	} else {
		RETURN(Cursor::NewInstance(isolate, cursor, true));
	}
}

METHOD(CreateCursor) {
	BEGIN();
	Local<v8::Uint8Array> data = args[0].As<Uint8Array>();
	Local<v8::Uint8Array> mask = args[1].As<Uint8Array>();
	INTARG(width, 2);
	INTARG(height, 3);
	INTARG(hotX, 4);
	INTARG(hotY, 5);
	SDL_Cursor *cursor = SDL_CreateCursor(
		static_cast<uint8_t*>(data->Buffer()->GetContents().Data()),
		static_cast<uint8_t*>(mask->Buffer()->GetContents().Data()),
		width, height,
		hotX, hotY
	);
	if (cursor == nullptr) {
		THROW_SDL_ERROR();
	} else {
		RETURN(Cursor::NewInstance(isolate, cursor, true));
	}
}

METHOD(CreateSystemCursor) {
	BEGIN();
	INTARG(cursorId, 0);
	SDL_Cursor *cursor = SDL_CreateSystemCursor((SDL_SystemCursor)cursorId);
	if (cursor == nullptr) {
		THROW_SDL_ERROR();
	} else {
		RETURN(Cursor::NewInstance(isolate, cursor, true));
	}
}

METHOD(FreeCursor) {
	BEGIN();
	UNWRAP(cursor, Cursor, args[0]);
	cursor->destroy();
}

METHOD(GetCursor) {
	BEGIN();
	SDL_Cursor *c = SDL_GetCursor();
	if (c == nullptr) {
		RETURN(MK_NULL());
	} else {
		RETURN(Cursor::NewInstance(isolate, c, false));
	}
}

METHOD(GetDefaultCursor) {
	BEGIN();
	SDL_Cursor *c = SDL_GetDefaultCursor();
	if (c == nullptr) {
		RETURN(MK_NULL());
	} else {
		RETURN(Cursor::NewInstance(isolate, c, false));
	}
}

METHOD(SetCursor) {
	BEGIN();
	UNWRAP(cursor, Cursor, args[0]);
	SDL_SetCursor(cursor->cursor_);
}

METHOD(CaptureMouse) {
	BEGIN();
	BOOLARG(capture, 0);
	if (SDL_CaptureMouse(capture ? SDL_TRUE : SDL_FALSE) < 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(GetRelativeMouseMode) {
    BEGIN();
    RETURN(MK_BOOL(SDL_GetRelativeMouseMode() == SDL_TRUE));
}

METHOD(SetRelativeMouseMode) {
    BEGIN();
    BOOLARG(rel, 0);
	if (SDL_SetRelativeMouseMode(rel ? SDL_TRUE : SDL_FALSE) < 0) {
		THROW_SDL_ERROR();
	}
}

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

METHOD(WarpMouseGlobal) {
	BEGIN();
	INTARG(x, 0);
	INTARG(y, 0);
	if (SDL_WarpMouseGlobal(x, y) < 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(WarpMouseInWindow) {
	BEGIN();
	UNWRAP(window, Window, args[0]);
	INTARG(x, 1);
	INTARG(y, 2);
	SDL_WarpMouseInWindow(window->window_, x, y);
}

METHOD(GetMouseState) {
	BEGIN();
	int x, y;
	auto buttons = SDL_GetMouseState(&x, &y);
	auto obj = MK_OBJECT();
	GET_CONTEXT();
	SET_KEY(obj, SYM(x), MK_NUMBER(x));
	SET_KEY(obj, SYM(y), MK_NUMBER(y));
	SET_KEY(obj, SYM(buttons), MK_NUMBER(buttons));
	RETURN(obj);
}

METHOD(GetGlobalMouseState) {
	BEGIN();
	int x, y;
	auto buttons = SDL_GetGlobalMouseState(&x, &y);
	auto obj = MK_OBJECT();
	GET_CONTEXT();
	SET_KEY(obj, SYM(x), MK_NUMBER(x));
	SET_KEY(obj, SYM(y), MK_NUMBER(y));
	SET_KEY(obj, SYM(buttons), MK_NUMBER(buttons));
	RETURN(obj);
}

METHOD(GetRelativeMouseState) {
	BEGIN();
	int x, y;
	auto buttons = SDL_GetRelativeMouseState(&x, &y);
	auto obj = MK_OBJECT();
	GET_CONTEXT();
	SET_KEY(obj, SYM(x), MK_NUMBER(x));
	SET_KEY(obj, SYM(y), MK_NUMBER(y));
	SET_KEY(obj, SYM(buttons), MK_NUMBER(buttons));
	RETURN(obj);
}

void InitMouseFunctions(Local<Object> exports) {
	NODE_SET_METHOD(exports, "createColorCursor", CreateColorCursor);
	NODE_SET_METHOD(exports, "createCursor", CreateCursor);
	NODE_SET_METHOD(exports, "createSystemCursor", CreateSystemCursor);
	NODE_SET_METHOD(exports, "freeCursor", FreeCursor);
	NODE_SET_METHOD(exports, "getCursor", GetCursor);
	NODE_SET_METHOD(exports, "getDefaultCursor", GetDefaultCursor);
	NODE_SET_METHOD(exports, "setCursor", SetCursor);
	NODE_SET_METHOD(exports, "captureMouse", CaptureMouse);
	NODE_SET_METHOD(exports, "getRelativeMouseMode", GetRelativeMouseMode);
	NODE_SET_METHOD(exports, "setRelativeMouseMode", SetRelativeMouseMode);
    NODE_SET_METHOD(exports, "showCursor", ShowCursor);
    NODE_SET_METHOD(exports, "warpMouseGlobal", WarpMouseGlobal);
    NODE_SET_METHOD(exports, "warpMouseInWindow", WarpMouseInWindow);
    NODE_SET_METHOD(exports, "getMouseState", GetMouseState);
    NODE_SET_METHOD(exports, "getGlobalMouseState", GetGlobalMouseState);
    NODE_SET_METHOD(exports, "getRelativeMouseState", GetRelativeMouseState);
}

}

#include "deps.h"

// TODO:
// SDL_GameControllerFromInstanceID
// SDL_GameControllerGetJoystick
// SDL_GameControllerMappingForGUID

// Not implemented:
// SDL_GameControllerAddMappingsFromRW

namespace sdl2_bindings {

using namespace v8;

METHOD(GameControllerAddMapping) {
	BEGIN();
	STRINGARG(mapping, 0);
	auto res = SDL_GameControllerAddMapping(*mapping);
	if (res < 0) {
		THROW_SDL_ERROR();
	} else {
		RETURN(MK_NUMBER(res));
	}
}

METHOD(GameControllerAddMappingsFromFile) {
	BEGIN();
	STRINGARG(file, 0);
	auto res = SDL_GameControllerAddMappingsFromFile(*file);
	if (res < 0) {
		THROW_SDL_ERROR();
	} else {
		RETURN(MK_NUMBER(res));
	}
}

METHOD(GameControllerEventState) {
	BEGIN();
	INTARG(state, 0);
	RETURN(MK_NUMBER(SDL_GameControllerEventState(state)));
}

// METHOD(GameControllerFromInstanceID) {

// }

METHOD(GameControllerGetAttached) {
	BEGIN();
	UNWRAP(gameController, GameController, args[0]);
	RETURN(MK_BOOL(SDL_GameControllerGetAttached(gameController->gameController_) == SDL_TRUE ? true : false));
}

METHOD(GameControllerGetAxis) {
	BEGIN();
	UNWRAP(gc, GameController, args[0]);
	INTARG(axis, 1);
	RETURN(MK_NUMBER(SDL_GameControllerGetAxis(gc->gameController_, (SDL_GameControllerAxis)axis)));
}

METHOD(GameControllerGetAxisFromString) {
	BEGIN();
	STRINGARG(axisName, 0);
	RETURN(MK_NUMBER(SDL_GameControllerGetAxisFromString(*axisName)));
}

METHOD(GameControllerGetBindForAxis) {
	BEGIN();
	UNWRAP(gc, GameController, args[0]);
	INTARG(axis, 1);
	auto bind = SDL_GameControllerGetBindForAxis(gc->gameController_, (SDL_GameControllerAxis)axis);
	auto out = MK_OBJECT();
	GET_CONTEXT();
	SET_KEY(out, SYM(bindType), MK_NUMBER(bind.bindType));
	SET_KEY(out, SYM(value), MK_NUMBER(bind.value.axis));
	RETURN(out);
}

METHOD(GameControllerGetBindForButton) {
	BEGIN();
	UNWRAP(gc, GameController, args[0]);
	INTARG(button, 1);
	auto bind = SDL_GameControllerGetBindForButton(gc->gameController_, (SDL_GameControllerButton)button);
	auto out = MK_OBJECT();
	GET_CONTEXT();
	SET_KEY(out, SYM(bindType), MK_NUMBER(bind.bindType));
	SET_KEY(out, SYM(value), MK_NUMBER(bind.value.button));
	RETURN(out);
}

METHOD(GameControllerGetButton) {
	BEGIN();
	UNWRAP(gc, GameController, args[0]);
	INTARG(button, 1);
	auto pressed = SDL_GameControllerGetButton(gc->gameController_, (SDL_GameControllerButton)button);
	RETURN(MK_BOOL(pressed == 1));
}

METHOD(GameControllerGetButtonFromString) {
	BEGIN();
	STRINGARG(buttonName, 0);
	RETURN(MK_NUMBER(SDL_GameControllerGetButtonFromString(*buttonName)));
}

// METHOD(GameControllerGetJoystick) {

// }

METHOD(GameControllerGetStringForAxis) {
	BEGIN();
	INTARG(axis, 0);
	auto str = SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)axis);
	if (str == nullptr) {
		RETURN(MK_NULL());
	} else {
		RETURN(MK_STRING(str));
	}
}

METHOD(GameControllerGetStringForButton) {
	BEGIN();
	INTARG(button, 0);
	auto str = SDL_GameControllerGetStringForButton((SDL_GameControllerButton)button);
	if (str == nullptr) {
		RETURN(MK_NULL());
	} else {
		RETURN(MK_STRING(str));
	}
}

METHOD(GameControllerMapping) {
	BEGIN();
	UNWRAP(gc, GameController, args[0]);
	auto mapping = SDL_GameControllerMapping(gc->gameController_);
	if (mapping == nullptr) {
		THROW_SDL_ERROR();
	} else {
		RETURN(MK_STRING(mapping));
	}
}

// METHOD(GameControllerMappingForGUID) {

// }

METHOD(GameControllerName) {
	BEGIN();
	UNWRAP(gc, GameController, args[0]);
	auto name = SDL_GameControllerName(gc->gameController_);
	if (name == nullptr) {
		RETURN(MK_NULL());
	} else {
		RETURN(MK_STRING(name));
	}
}

METHOD(GameControllerNameForIndex) {
	BEGIN();
	INTARG(ix, 0);
	auto name = SDL_GameControllerNameForIndex(ix);
	if (name == nullptr) {
		RETURN(MK_NULL());
	} else {
		RETURN(MK_STRING(name));
	}
}

METHOD(GameControllerOpen) {
	BEGIN();
	INTARG(ix, 0);
	auto gc = SDL_GameControllerOpen(ix);
	if (gc == nullptr) {
		THROW_SDL_ERROR();
	} else {
		RETURN(GameController::NewInstance(isolate, gc));
	}
}

METHOD(GameControllerUpdate) {
	SDL_GameControllerUpdate();
}

METHOD(IsGameController) {
	BEGIN();
	INTARG(index, 0);
	RETURN(MK_BOOL(SDL_IsGameController(index) == SDL_TRUE ? true : false));
}

void InitGameControllerFunctions(Local<Object> exports) {
	NODE_SET_METHOD(exports, "gameControllerAddMapping", GameControllerAddMapping);
	NODE_SET_METHOD(exports, "gameControllerAddMappingsFromFile", GameControllerAddMappingsFromFile);
	NODE_SET_METHOD(exports, "gameControllerEventState", GameControllerEventState);
	// NODE_SET_METHOD(exports, "gameControllerFromInstanceID", GameControllerFromInstanceID);
	NODE_SET_METHOD(exports, "gameControllerGetAttached", GameControllerGetAttached);
	NODE_SET_METHOD(exports, "gameControllerGetAxis", GameControllerGetAxis);
	NODE_SET_METHOD(exports, "gameControllerGetAxisFromString", GameControllerGetAxisFromString);
	NODE_SET_METHOD(exports, "gameControllerGetBindForAxis", GameControllerGetBindForAxis);
	NODE_SET_METHOD(exports, "gameControllerGetBindForButton", GameControllerGetBindForButton);
	NODE_SET_METHOD(exports, "gameControllerGetButton", GameControllerGetButton);
	NODE_SET_METHOD(exports, "gameControllerGetButtonFromString", GameControllerGetButtonFromString);
	// NODE_SET_METHOD(exports, "gameControllerGetJoystick", GameControllerGetJoystick);
	NODE_SET_METHOD(exports, "gameControllerGetStringForAxis", GameControllerGetStringForAxis);
	NODE_SET_METHOD(exports, "gameControllerGetStringForButton", GameControllerGetStringForButton);
	NODE_SET_METHOD(exports, "gameControllerMapping", GameControllerMapping);
	// NODE_SET_METHOD(exports, "gameControllerMappingForGUID", GameControllerMappingForGUID);
	NODE_SET_METHOD(exports, "gameControllerName", GameControllerName);
	NODE_SET_METHOD(exports, "gameControllerNameForIndex", GameControllerNameForIndex);
	NODE_SET_METHOD(exports, "gameControllerOpen", GameControllerOpen);
	NODE_SET_METHOD(exports, "gameControllerUpdate", GameControllerUpdate);
    NODE_SET_METHOD(exports, "isGameController", IsGameController);
}

}

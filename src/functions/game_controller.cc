#include "deps.h"

// Not implemented:
// SDL_GameControllerAddMappingsFromRW

namespace sdl2_bindings {

using namespace v8;

METHOD(GameControllerAddMapping) {

}

METHOD(GameControllerAddMappingsFromFile) {

}

METHOD(GameControllerEventState) {

}

METHOD(GameControllerFromInstanceID) {

}

METHOD(GameControllerGetAttached) {

}

METHOD(GameControllerGetAxis) {

}

METHOD(GameControllerGetAxisFromString) {

}

METHOD(GameControllerGetBindForAxis) {

}

METHOD(GameControllerGetBindForButton) {

}

METHOD(GameControllerGetButton) {

}

METHOD(GameControllerGetButtonFromString) {

}

METHOD(GameControllerGetJoystick) {

}

METHOD(GameControllerGetStringForAxis) {

}

METHOD(GameControllerGetStringForButton) {

}

METHOD(GameControllerMapping) {

}

METHOD(GameControllerMappingForGUID) {

}

METHOD(GameControllerName) {

}

METHOD(GameControllerNameForIndex) {

}

METHOD(GameControllerOpen) {

}

METHOD(GameControllerUpdate) {

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
	NODE_SET_METHOD(exports, "gameControllerFromInstanceID", GameControllerFromInstanceID);
	NODE_SET_METHOD(exports, "gameControllerGetAttached", GameControllerGetAttached);
	NODE_SET_METHOD(exports, "gameControllerGetAxis", GameControllerGetAxis);
	NODE_SET_METHOD(exports, "gameControllerGetAxisFromString", GameControllerGetAxisFromString);
	NODE_SET_METHOD(exports, "gameControllerGetBindForAxis", GameControllerGetBindForAxis);
	NODE_SET_METHOD(exports, "gameControllerGetBindForButton", GameControllerGetBindForButton);
	NODE_SET_METHOD(exports, "gameControllerGetButton", GameControllerGetButton);
	NODE_SET_METHOD(exports, "gameControllerGetButtonFromString", GameControllerGetButtonFromString);
	NODE_SET_METHOD(exports, "gameControllerGetJoystick", GameControllerGetJoystick);
	NODE_SET_METHOD(exports, "gameControllerGetStringForAxis", GameControllerGetStringForAxis);
	NODE_SET_METHOD(exports, "gameControllerGetStringForButton", GameControllerGetStringForButton);
	NODE_SET_METHOD(exports, "gameControllerMapping", GameControllerMapping);
	NODE_SET_METHOD(exports, "gameControllerMappingForGUID", GameControllerMappingForGUID);
	NODE_SET_METHOD(exports, "gameControllerName", GameControllerName);
	NODE_SET_METHOD(exports, "gameControllerNameForIndex", GameControllerNameForIndex);
	NODE_SET_METHOD(exports, "gameControllerOpen", GameControllerOpen);
	NODE_SET_METHOD(exports, "gameControllerUpdate", GameControllerUpdate);
    NODE_SET_METHOD(exports, "isGameController", IsGameController);
}

}

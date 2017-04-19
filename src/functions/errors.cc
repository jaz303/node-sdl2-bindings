#include "deps.h"

// Not implemented:
// SDL_SetError - not sure how to handle varargs

namespace sdl2_bindings {

using namespace v8;

METHOD(ClearError) {
	SDL_ClearError();
}

METHOD(GetError) {
	BEGIN();
	RETURN(MK_STRING(SDL_GetError()));
}

void InitErrorFunctions(Local<Object> exports) {
	NODE_SET_METHOD(exports, "clearError", ClearError);    
	NODE_SET_METHOD(exports, "getError", GetError);
}

}

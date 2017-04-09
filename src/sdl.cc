#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

void SDL2BindingsInit(Local<Object> exports) {
	InitSymbols(exports->GetIsolate());
	InitConstants(exports);

	InitCoreFunctions(exports);
	InitDisplayWindowFunctions(exports);
	InitEventFunctions(exports);
	InitExtImageFunctions(exports);
	InitGLFunctions(exports);
	InitJoystickFunctions(exports);
	InitPixelFormatConversionFunctions(exports);
	InitSurfaceDrawingFunctions(exports);

	GLContext::Init(exports->GetIsolate());
	Surface::Init(exports->GetIsolate());
	Window::Init(exports->GetIsolate());
}

NODE_MODULE(sdl2_bindings, SDL2BindingsInit);

}

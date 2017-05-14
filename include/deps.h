#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "macros.h"

namespace sdl2_bindings {
	#define DEFSYM(name) extern v8::Persistent<v8::String> sym_##name;
	#include "symbols.x"
	#undef DEFSYM
}

#include "types.h"

namespace sdl2_bindings {

	void attachWindowReference(SDL_Window *sdlWindow, Window *window);
    Window* getWindowReference(SDL_Window *sdlWindow);
	void extractPoint(Isolate *isolate, Local<Object> obj, SDL_Point *point);
	void extractRect(Isolate *isolate, Local<Object> obj, SDL_Rect *rect);
	void populateRect(Isolate *isolate, Local<Object> obj, SDL_Rect *rect);
	void extractDisplayMode(Isolate *isolate, Local<Object> obj, SDL_DisplayMode *dm);
	void populateDisplayMode(Isolate *isolate, Local<Object> obj, SDL_DisplayMode *dm);

	void InitSymbols(v8::Isolate *isolate);
	void InitConstants(v8::Local<v8::Object> exports);

	void Init2DAcceleratedRenderingFunctions(Local<Object> exports);
	void InitClipboardFunctions(v8::Local<v8::Object> exports);
	void InitCoreFunctions(v8::Local<v8::Object> exports);
	void InitCPUFeatureDetectionFunctions(Local<Object> exports);
	void InitDisplayWindowFunctions(Local<Object> exports);
	void InitErrorFunctions(Local<Object> exports);
	void InitEventFunctions(v8::Local<v8::Object> exports);
	void InitExtImageFunctions(v8::Local<v8::Object> exports);
	void InitGameControllerFunctions(Local<Object> exports);
	void InitGLFunctions(v8::Local<v8::Object> exports);
	void InitJoystickFunctions(v8::Local<v8::Object> exports);
	void InitKeyboardFunctions(v8::Local<v8::Object> exports);
	void InitMouseFunctions(v8::Local<v8::Object> exports);
	void InitPixelFormatConversionFunctions(v8::Local<v8::Object> exports);
	void InitSurfaceDrawingFunctions(v8::Local<v8::Object> exports);
	void InitTimerFunctions(v8::Local<v8::Object> exports);
}

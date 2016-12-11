#include <string>
#include <iostream>

// TODO: optimise event loading

#include "deps.h"
#include "macros.h"
#include "types.h"

namespace nsdl {

using namespace v8;

using v8::Boolean;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Number;
using v8::Exception;
using v8::Array;
using v8::Context;

// Prototypes
void initConstants(Local<Object> exports);

// Symbols
#define DEFSYM(name) v8::Persistent<v8::String> sym_##name;
#include "symbols.x"
#undef DEFSYM

//
// Helpers

void extractRect(Isolate *isolate, Local<Object> obj, SDL_Rect *rect) {
	auto ctx = isolate->GetCurrentContext();
	rect->x = obj->Get(ctx, SYM(x)).ToLocalChecked()->Int32Value();
	rect->y = obj->Get(ctx, SYM(y)).ToLocalChecked()->Int32Value();
	rect->w = obj->Get(ctx, SYM(width)).ToLocalChecked()->Int32Value();
	rect->h = obj->Get(ctx, SYM(height)).ToLocalChecked()->Int32Value();
}

//
// Window

v8::Persistent<v8::Function> Window::constructor;

Window::Window(SDL_Window *window) : window_(window) {}
Window::~Window() {
	destroy();
}

void Window::destroy() {
	if (window_) {
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}
}

void Window::Init(Isolate *isolate) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Window"));
	tpl->InstanceTemplate()->SetInternalFieldCount(2);
	DEFINE_METHOD(tpl, "update", Update);
	DEFINE_GETTER(tpl, "surface", GetSurface);
	DEFINE_ACCESSOR(tpl, "title", GetTitle, SetTitle);
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

METHOD(Window::New) {}

METHOD(Window::Update) {
	UNWRAP_ME(w, Window);
	SDL_UpdateWindowSurface(w->window_);
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

GETTER(Window::GetTitle) {
	BEGIN();
	UNWRAP_ME(w, Window);
	RETURN(MK_STRING(SDL_GetWindowTitle(w->window_)));
}

SETTER(Window::SetTitle) {
	BEGIN();
	String::Utf8Value newTitle(value);
	UNWRAP_ME(w, Window);
	SDL_SetWindowTitle(w->window_, *newTitle);
}

//
// Surface

v8::Persistent<v8::Function> Surface::constructor;

Surface::Surface(SDL_Surface *surface, bool owned) : surface_(surface), owned_(owned) {}
Surface::~Surface() {
	if (owned_) {
		SDL_FreeSurface(surface_);
	}
}

void Surface::Init(Isolate *isolate) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Surface"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	DEFINE_METHOD(tpl, "blit", Blit);
	DEFINE_METHOD(tpl, "blitRect", BlitRect);
	DEFINE_METHOD(tpl, "blitRectScaled", BlitRectScaled);
	DEFINE_METHOD(tpl, "blitScaled", BlitScaled);
	DEFINE_METHOD(tpl, "clear", Clear);
	DEFINE_METHOD(tpl, "fillRect", FillRect);
	DEFINE_METHOD(tpl, "mapRGB", MapRGB);
	DEFINE_METHOD(tpl, "mapRGBA", MapRGBA);
	DEFINE_GETTER(tpl, "width", GetWidth);
	DEFINE_GETTER(tpl, "height", GetHeight);
	DEFINE_GETTER(tpl, "pitch", GetPitch);
	constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> Surface::NewInstance(Isolate *isolate, SDL_Surface *surface, bool owned) {
	Local<Function> cons = Local<Function>::New(isolate, constructor);
	Local<Context> context = isolate->GetCurrentContext();
	Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
	Surface *s = new Surface(surface, owned);
	s ->Wrap(instance);
	return instance;
}

METHOD(Surface::New) {}

// blit(src, x, y)
// blit(src, sx, sy, sw, sh, x, y)
METHOD(Surface::Blit) {
	BEGIN();

	UNWRAP_ME(dst, Surface);
	UNWRAP(src, Surface, args[0]);

	SDL_Surface *ds = dst->surface_, *ss = src->surface_;
	SDL_Rect dr, sr;

	switch (args.Length()) {
		case 3:
		{
			dr.x = args[1]->Int32Value();
			dr.y = args[2]->Int32Value();
			SDL_BlitSurface(ss, NULL, ds, &dr);
			break;
		}
		case 7:
		{
			sr.x = args[1]->Int32Value();
			sr.y = args[2]->Int32Value();
			sr.w = args[3]->Int32Value();
			sr.h = args[4]->Int32Value();
			dr.x = args[5]->Int32Value();
			dr.y = args[6]->Int32Value();
			SDL_BlitSurface(ss, &sr, ds, &dr);
			break;
		}
		default:
		{
			THROW(TypeError, "argument error");
		}
	}
}

// blitRect(src, srcRect, destRect)
// blitRect(src, srcRect, x, y)
METHOD(Surface::BlitRect) {
	BEGIN();
	THROW(Error, "not implemented");
}

// blitRectScaled(src, srcRect, destRect)
// blitRectScaled(src, srcRect, x, y)
// blitRectScaled(src, srcRect, x, y, w, h)
METHOD(Surface::BlitRectScaled) {
	BEGIN();
	THROW(Error, "not implemented");
}

// 1 blitScaled(src) - fill destination with src
// 5 blitScaled(src, x, y, w, h) - blit src into x,y,w,h
// 9 blitScaled(src, sx, sy, sw, sh, x, y, w, h)
METHOD(Surface::BlitScaled) {
	BEGIN();

	UNWRAP_ME(dst, Surface);
	UNWRAP(src, Surface, args[0]);

	SDL_Surface *ds = dst->surface_, *ss = src->surface_;
	SDL_Rect dr, sr;

	switch (args.Length()) {
		case 1:
		{
			SDL_BlitScaled(ss, NULL, ds, NULL);
			break;
		}
		case 5:
		{
			dr.x = args[1]->Int32Value();
			dr.y = args[2]->Int32Value();
			dr.w = args[3]->Int32Value();
			dr.h = args[4]->Int32Value();
			SDL_BlitScaled(ss, NULL, ds, &dr);
			break;
		}
		case 9:
		{
			sr.x = args[1]->Int32Value();
			sr.y = args[2]->Int32Value();
			sr.w = args[3]->Int32Value();
			sr.h = args[4]->Int32Value();
			dr.x = args[5]->Int32Value();
			dr.y = args[6]->Int32Value();
			dr.w = args[7]->Int32Value();
			dr.h = args[8]->Int32Value();
			SDL_BlitScaled(ss, &sr, ds, &dr);
			break;
		}
		default:
		{
			THROW(TypeError, "argument error");
		}
	}
}

METHOD(Surface::Clear) {
	BEGIN();
	NARGS(1);
	UINT32ARG(color, 0);
	UNWRAP_ME(s, Surface);
	SDL_FillRect(s->surface_, NULL, color);
}

METHOD(Surface::FillRect) {
	BEGIN();
	UNWRAP_ME(s, Surface);
	uint32_t color;
	SDL_Rect r;
	if (args.Length() == 2) {
		// FIXME: typecheck needed?
		extractRect(isolate, args[0]->ToObject(), &r);
		color = args[1]->Uint32Value();
	} else if (args.Length() == 5) {
		r.x = args[0]->Int32Value();
		r.y = args[1]->Int32Value();
		r.w = args[2]->Int32Value();
		r.h = args[3]->Int32Value();
		color = args[4]->Uint32Value();
	} else {
		THROW(TypeError, "argument error");
	}
	SDL_FillRect(s->surface_, &r, color);
}

METHOD(Surface::MapRGB) {
	BEGIN();
	NARGS(3);
	INTARG(r, 0);
	INTARG(g, 1);
	INTARG(b, 2);
	UNWRAP_ME(s, Surface);
	RETURN(MK_NUMBER(SDL_MapRGB(s->surface_->format, r, g, b)));
}

METHOD(Surface::MapRGBA) {
	BEGIN();
	NARGS(4);
	INTARG(r, 0);
	INTARG(g, 1);
	INTARG(b, 2);
	INTARG(a, 3);
	UNWRAP_ME(s, Surface);
	RETURN(MK_NUMBER(SDL_MapRGBA(s->surface_->format, r, g, b, a)));
}

GETTER(Surface::GetWidth) {
	BEGIN();
	UNWRAP(s, Surface, args.This());
	RETURN(MK_NUMBER(s->surface_->w));
}

GETTER(Surface::GetHeight) {
	BEGIN();
	UNWRAP(s, Surface, args.This());
	RETURN(MK_NUMBER(s->surface_->h));	
}

GETTER(Surface::GetPitch) {
	BEGIN();
	UNWRAP(s, Surface, args.This());
	RETURN(MK_NUMBER(s->surface_->pitch));
}

//
//

METHOD(Init) {
	BEGIN();
	if (args.Length() != 1
		|| !args[0]->IsNumber()) {
		THROW(TypeError, "argument error");
	}
	auto res = SDL_Init(UINT32(args[0]));
	args.GetReturnValue().Set(Boolean::New(isolate, res == 0));
}

METHOD(CreateWindow) {
	BEGIN();

	NARGS(6);
	STRINGARG(title, 0);
	INTARG(x, 1);
	INTARG(y, 2);
	INTARG(w, 3);
	INTARG(h, 4);
	UINT32ARG(flags, 5);

	SDL_Window *win = SDL_CreateWindow(*title, x, y, w, h, flags);
	if (!win) {
		THROW(Error, "couldn't create window");
	}

	RETURN(Window::NewInstance(isolate, win));
}

METHOD(DestroyWindow) {
	BEGIN();
	UNWRAP(w, Window, args[0]);
	w->destroy();
}

METHOD(SetWindowSize) {
	BEGIN();
	NARGS(3);
	INTARG(w, 1);
	INTARG(h, 2);
	UNWRAP(win, Window, args[0]);
	SDL_SetWindowSize(win->window_, w, h);
}

//
// Events

#define EVKEY(name) \
	Local<Object> evinfo = Object::New(isolate); \
	evt->CreateDataProperty(ctx, SYM(name), evinfo)

#define EVSET(key, value) \
	evinfo->CreateDataProperty(ctx, SYM(key), value)

void populateEvent(Isolate *isolate, Local<Object> evt, SDL_Event *sdlEvent) {
	auto ctx = isolate->GetCurrentContext();
	evt->CreateDataProperty(ctx, SYM(type), Number::New(isolate, sdlEvent->type));
	switch (sdlEvent->type) {
		case SDL_WINDOWEVENT:
		{
			break;
		}
		case SDL_SYSWMEVENT:
		{
			break;
		}
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			break;
		}
		case SDL_MOUSEMOTION:
		{
			EVKEY(motion);
			EVSET(timestamp, MK_NUMBER(sdlEvent->motion.timestamp));
			EVSET(windowId, MK_NUMBER(sdlEvent->motion.windowID));
			EVSET(which, MK_NUMBER(sdlEvent->motion.which));
			EVSET(state, MK_NUMBER(sdlEvent->motion.state));
			EVSET(x, MK_NUMBER(sdlEvent->motion.x));
			EVSET(y, MK_NUMBER(sdlEvent->motion.y));
			EVSET(xRel, MK_NUMBER(sdlEvent->motion.xrel));
			EVSET(yRel, MK_NUMBER(sdlEvent->motion.yrel));
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		{
			EVKEY(button);
			EVSET(timestamp, MK_NUMBER(sdlEvent->button.timestamp));
			EVSET(windowId, MK_NUMBER(sdlEvent->button.windowID));
			EVSET(which, MK_NUMBER(sdlEvent->button.which));
			EVSET(button, MK_NUMBER(sdlEvent->button.button));
			EVSET(state, MK_NUMBER(sdlEvent->button.state));
			EVSET(clicks, MK_NUMBER(sdlEvent->button.clicks));
			EVSET(x, MK_NUMBER(sdlEvent->button.x));
			EVSET(y, MK_NUMBER(sdlEvent->button.y));
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			break;
		}
		case SDL_FINGERDOWN:
		case SDL_FINGERUP:
		{
			break;
		}
		case SDL_FINGERMOTION:
		{
			break;
		}
		case SDL_JOYAXISMOTION:
		{
			EVKEY(jaxis);
			EVSET(timestamp, MK_NUMBER(sdlEvent->jaxis.timestamp));
			EVSET(which, MK_NUMBER(sdlEvent->jaxis.which));
			EVSET(axis, MK_NUMBER(sdlEvent->jaxis.axis));
			EVSET(value, MK_NUMBER(sdlEvent->jaxis.value));
			break;
		}
		case SDL_JOYBALLMOTION:
		{
			EVKEY(jball);
			EVSET(timestamp, MK_NUMBER(sdlEvent->jball.timestamp));
			EVSET(which, MK_NUMBER(sdlEvent->jball.which));
			EVSET(ball, MK_NUMBER(sdlEvent->jball.ball));
			EVSET(xRel, MK_NUMBER(sdlEvent->jball.xrel));
			EVSET(yRel, MK_NUMBER(sdlEvent->jball.yrel));
			break;
		}
		case SDL_JOYHATMOTION:
		{
			EVKEY(jhat);
			EVSET(timestamp, MK_NUMBER(sdlEvent->jhat.timestamp));
			EVSET(which, MK_NUMBER(sdlEvent->jhat.which));
			EVSET(hat, MK_NUMBER(sdlEvent->jhat.hat));
			EVSET(value, MK_NUMBER(sdlEvent->jhat.value));
			break;
		}
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		{
			EVKEY(jbutton);
			EVSET(timestamp, MK_NUMBER(sdlEvent->jbutton.timestamp));
			EVSET(which, MK_NUMBER(sdlEvent->jbutton.which));
			EVSET(button, MK_NUMBER(sdlEvent->jbutton.button));
			EVSET(state, MK_NUMBER(sdlEvent->jbutton.state));
			break;
		}
		case SDL_JOYDEVICEADDED:
		case SDL_JOYDEVICEREMOVED:
		{
			EVKEY(jdevice);
			EVSET(timestamp, MK_NUMBER(sdlEvent->jdevice.timestamp));
			EVSET(which, MK_NUMBER(sdlEvent->jdevice.which));
			break;
		}
	}
}

// SDL_AddEventWatch
// SDL_DelEventWatch
// SDL_EventState
// SDL_FilterEvents
// SDL_FlushEvent
// SDL_FlushEvents
// SDL_GetEventFilter
// SDL_GetEventState
// SDL_GetNumTouchDevices
// SDL_GetNumTouchFingers
// SDL_GetTouchDevice
// SDL_GetTouchFinger
// SDL_HasEvent
// SDL_HasEvents
// SDL_LoadDollarTemplates
// SDL_PeepEvents

METHOD(PollEvent) {
	BEGIN();

	NARGS(1);
	OBJECTARG(evt, 0);

	SDL_Event sdlEvent;
	if (SDL_PollEvent(&sdlEvent)) {
		populateEvent(isolate, evt, &sdlEvent);
		RETURN(MK_TRUE());
	} else {
		RETURN(MK_FALSE());
	}
}

METHOD(PumpEvents) {
	SDL_PumpEvents();
}

// SDL_PushEvent

METHOD(QuitRequested) {
	BEGIN();
	RETURN(MK_BOOL(SDL_QuitRequested()));
}

// SDL_RecordGesture
// SDL_RegisterEvents
// SDL_SaveAllDollarTemplates
// SDL_SaveDollarTemplate
// SDL_SetEventFilter

METHOD(WaitEvent) {
	BEGIN();

	NARGS2(1, 2);
	OBJECTARG(evt, 0);

	SDL_Event sdlEvent;
	if (args.Length() == 2) {
		INTARG(timeout, 1);
		int res = SDL_WaitEventTimeout(&sdlEvent, timeout);
		if (res) {
			populateEvent(isolate, evt, &sdlEvent);
			RETURN(MK_TRUE());
		} else {
			RETURN(MK_FALSE());
		}
	} else {
		SDL_WaitEvent(&sdlEvent);	
		populateEvent(isolate, evt, &sdlEvent);
	}
}

//
//

SDL_Joystick* extractJoystick(Isolate *isolate, int id) {
	SDL_Joystick *js = SDL_JoystickFromInstanceID(id);
	if (js == nullptr) {
		isolate->ThrowException(Exception::Error(
			String::NewFromUtf8(isolate, "invalid joystick ID")
		));
	}
	return js;
}

METHOD(JoystickClose) {
	BEGIN();
	NARGS(1);
	INTARG(joystickId, 0);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	SDL_JoystickClose(js);
}

METHOD(JoystickCurrentPowerLevel) {
	BEGIN();
	NARGS(1);
	INTARG(joystickId, 0);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	RETURN(MK_NUMBER(SDL_JoystickCurrentPowerLevel(js)));
}

METHOD(JoystickEventState) {
	BEGIN();
	NARGS(1);
	INTARG(state, 0);
	RETURN(MK_BOOL(SDL_JoystickEventState(state)));	
}

// METHOD(JoystickFromInstanceID) {
// 	// TODO
// }

METHOD(JoystickGetAttached) {
	BEGIN();
	NARGS(1);
	INTARG(joystickId, 0);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	RETURN(MK_BOOL(SDL_JoystickGetAttached(js)));
}

METHOD(JoystickGetAxis) {
	BEGIN();
	NARGS(2);
	INTARG(joystickId, 0);
	INTARG(axis, 1);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	RETURN(MK_NUMBER(SDL_JoystickGetHat(js, axis)));
}

METHOD(JoystickGetBall) {
	BEGIN();
	NARGS(2);
	INTARG(joystickId, 0);
	INTARG(ball, 1);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	RETURN(MK_NUMBER(SDL_JoystickGetHat(js, ball)));
}

METHOD(JoystickGetButton) {
	BEGIN();
	NARGS(2);
	INTARG(joystickId, 0);
	INTARG(button, 1);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	RETURN(MK_NUMBER(SDL_JoystickGetButton(js, button)));
}

// METHOD(JoystickGetDeviceGUID) {
// 	BEGIN();
// 	NARGS(1);
// 	INTARG(deviceIndex, 0);
// 	RETURN(MK_NUMBER(SDL_JoystickGetDeviceGUID(deviceIndex)));	
// }

// METHOD(JoystickGetGUID) {
// 	// TODO
// }

// METHOD(JoystickGetGUIDFromString) {
// 	// TODO
// }

// METHOD(JoystickGetGUIDString) {
// 	// TODO
// }

METHOD(JoystickGetHat) {
	BEGIN();
	NARGS(2);
	INTARG(joystickId, 0);
	INTARG(hat, 1);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	RETURN(MK_NUMBER(SDL_JoystickGetHat(js, hat)));
}

METHOD(JoystickInstanceID) {
	BEGIN();
	NARGS(1);
	RETURN(args[0]);
}

METHOD(JoystickName) {
	BEGIN();
	NARGS(1);
	INTARG(joystickId, 0);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	RETURN(MK_STRING(SDL_JoystickName(js)));
}

METHOD(JoystickNameForIndex) {
	BEGIN();
	NARGS(1);
	INTARG(index, 0);
	RETURN(MK_STRING(SDL_JoystickNameForIndex(index)));
}

METHOD(JoystickNumAxes) {
	BEGIN();
	NARGS(1);
	INTARG(joystickId, 0);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	RETURN(MK_NUMBER(SDL_JoystickNumAxes(js)));
}

METHOD(JoystickNumBalls) {
	BEGIN();
	NARGS(1);
	INTARG(joystickId, 0);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	RETURN(MK_NUMBER(SDL_JoystickNumBalls(js)));
}

METHOD(JoystickNumButtons) {
	BEGIN();
	NARGS(1);
	INTARG(joystickId, 0);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	RETURN(MK_NUMBER(SDL_JoystickNumButtons(js)));
}

METHOD(JoystickNumHats) {
	BEGIN();
	NARGS(1);
	INTARG(joystickId, 0);
	SDL_Joystick *js = extractJoystick(isolate, joystickId);
	if (!js) return;
	RETURN(MK_NUMBER(SDL_JoystickNumHats(js)));
}

METHOD(JoystickOpen) {
	BEGIN();
	NARGS(1);
	INTARG(index, 0);
	SDL_Joystick *joystick = SDL_JoystickOpen(index);
	if (joystick == nullptr) {
		THROW(Error, "couldn't open joystick");
	}
	RETURN(MK_NUMBER(SDL_JoystickInstanceID(joystick)));
}

METHOD(JoystickUpdate) {
	SDL_JoystickUpdate();
}

METHOD(NumJoysticks) {
	BEGIN();
	RETURN(MK_NUMBER(SDL_NumJoysticks()));
}

//
// SDL2_image

METHOD(ImageInit) {
	BEGIN();
	int flags;
	if (args.Length() == 0) {
		flags = IMG_INIT_PNG | IMG_INIT_JPG;
	} else {
		NARGS(1);
		flags = args[0]->Int32Value();
	}
	int res = IMG_Init(flags);
	RETURN(MK_NUMBER(res));
}

METHOD(ImageQuit) {
	IMG_Quit();
}

METHOD(ImageLoad) {
	BEGIN();
	NARGS(1);
	STRINGARG(file, 0);
	SDL_Surface *image = IMG_Load(*file);
	if (image == nullptr) {
		THROW(Error, IMG_GetError());
	} else {
		RETURN(Surface::NewInstance(isolate, image, true));
	}
}

//
//

void SDL2ModuleInit(Local<Object> exports) {
	Isolate *isolate = exports->GetIsolate();

	#define DEFSYM(name) sym_##name.Reset(isolate, String::NewFromUtf8(isolate, #name));
	#include "symbols.x"
	#undef DEFSYM

	initConstants(exports);

	Window::Init(isolate);
	Surface::Init(isolate);

	NODE_SET_METHOD(exports, "init", Init);
	NODE_SET_METHOD(exports, "createWindow", CreateWindow);
	NODE_SET_METHOD(exports, "destroyWindow", DestroyWindow);
	NODE_SET_METHOD(exports, "setWindowSize", SetWindowSize);

	// Events
	NODE_SET_METHOD(exports, "pollEvent", PollEvent);
	NODE_SET_METHOD(exports, "pumpEvents", PumpEvents);
	NODE_SET_METHOD(exports, "quitRequested", QuitRequested);
	NODE_SET_METHOD(exports, "waitEvent", WaitEvent);

	// Josticks
	NODE_SET_METHOD(exports, "joystickClose", JoystickClose);
	NODE_SET_METHOD(exports, "joystickCurrentPowerLevel", JoystickCurrentPowerLevel);
	NODE_SET_METHOD(exports, "joystickEventState", JoystickEventState);
	// NODE_SET_METHOD(exports, "joystickFromInstanceID", JoystickFromInstanceID);
	NODE_SET_METHOD(exports, "joystickGetAttached", JoystickGetAttached);
	NODE_SET_METHOD(exports, "joystickGetAxis", JoystickGetAxis);
	NODE_SET_METHOD(exports, "joystickGetBall", JoystickGetBall);
	NODE_SET_METHOD(exports, "joystickGetButton", JoystickGetButton);
	// NODE_SET_METHOD(exports, "joystickGetDeviceGUID", JoystickGetDeviceGUID);
	// NODE_SET_METHOD(exports, "joystickGetGUID", JoystickGetGUID);
	// NODE_SET_METHOD(exports, "joystickGetGUIDFromString", JoystickGetGUIDFromString);
	// NODE_SET_METHOD(exports, "joystickGetGUIDString", JoystickGetGUIDString);
	NODE_SET_METHOD(exports, "joystickGetHat", JoystickGetHat);
	NODE_SET_METHOD(exports, "joystickInstanceID", JoystickInstanceID);
	NODE_SET_METHOD(exports, "joystickName", JoystickName);
	NODE_SET_METHOD(exports, "joystickNameForIndex", JoystickNameForIndex);
	NODE_SET_METHOD(exports, "joystickNumAxes", JoystickNumAxes);
	NODE_SET_METHOD(exports, "joystickNumBalls", JoystickNumBalls);
	NODE_SET_METHOD(exports, "joystickNumButtons", JoystickNumButtons);
	NODE_SET_METHOD(exports, "joystickNumHats", JoystickNumHats);
	NODE_SET_METHOD(exports, "joystickOpen", JoystickOpen);
	NODE_SET_METHOD(exports, "joystickUpdate", JoystickUpdate);
	NODE_SET_METHOD(exports, "numJoysticks", NumJoysticks);

	// Images
	NODE_SET_METHOD(exports, "imageInit", ImageInit);
	NODE_SET_METHOD(exports, "imageQuit", ImageQuit);
	NODE_SET_METHOD(exports, "imageLoad", ImageLoad);

}

NODE_MODULE(sdl2, SDL2ModuleInit);

}
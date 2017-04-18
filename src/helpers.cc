#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

void extractRect(Isolate *isolate, Local<Object> obj, SDL_Rect *rect) {
    GET_CONTEXT();
    rect->x = obj->Get(ctx, SYM(x)).ToLocalChecked()->Int32Value();
    rect->y = obj->Get(ctx, SYM(y)).ToLocalChecked()->Int32Value();
    rect->w = obj->Get(ctx, SYM(width)).ToLocalChecked()->Int32Value();
    rect->h = obj->Get(ctx, SYM(height)).ToLocalChecked()->Int32Value();
}

void populateRect(Isolate *isolate, Local<Object> obj, SDL_Rect *rect) {
    GET_CONTEXT();
    SET_KEY(obj, SYM(x), MK_NUMBER(rect->x));
    SET_KEY(obj, SYM(y), MK_NUMBER(rect->y));
    SET_KEY(obj, SYM(width), MK_NUMBER(rect->w));
    SET_KEY(obj, SYM(height), MK_NUMBER(rect->h));
}

void extractDisplayMode(Isolate *isolate, Local<Object> obj, SDL_DisplayMode *dm) {
    GET_CONTEXT();
    dm->format = obj->Get(ctx, SYM(format)).ToLocalChecked()->Uint32Value();
    dm->w = obj->Get(ctx, SYM(width)).ToLocalChecked()->Int32Value();
    dm->h = obj->Get(ctx, SYM(height)).ToLocalChecked()->Int32Value();
    dm->refresh_rate = obj->Get(ctx, SYM(refreshRate)).ToLocalChecked()->Int32Value();
    dm->driverdata = nullptr;
}

void populateDisplayMode(Isolate *isolate, Local<Object> obj, SDL_DisplayMode *dm) {
	GET_CONTEXT();
    SET_KEY(obj, SYM(format), MK_NUMBER(dm->format));
    SET_KEY(obj, SYM(width), MK_NUMBER(dm->w));
    SET_KEY(obj, SYM(height), MK_NUMBER(dm->h));
    SET_KEY(obj, SYM(refreshRate), MK_NUMBER(dm->refresh_rate));
}

}

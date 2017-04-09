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

}

#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

METHOD(BlitScaled) {
	BEGIN();
	
	UNWRAP(src, Surface, args[0]);
	SDL_Rect srcRect;
	SDL_Rect *srcRectPtr = nullptr;
	if (!args[1]->IsNull()) {
		extractRect(isolate, args[1]->ToObject(), &srcRect);
		srcRectPtr = &srcRect;
	}

	UNWRAP(dst, Surface, args[2]);
	SDL_Rect dstRect;
	SDL_Rect *dstRectPtr = nullptr;
	if (!args[3]->IsNull()) {
		extractRect(isolate, args[3]->ToObject(), &dstRect);
		dstRectPtr = &dstRect;
	}

	RETURN(MK_NUMBER(SDL_BlitScaled(src->surface_, srcRectPtr, dst->surface_, dstRectPtr)));
}

METHOD(BlitSurface) {
	BEGIN();
	
	UNWRAP(src, Surface, args[0]);
	SDL_Rect srcRect;
	SDL_Rect *srcRectPtr = nullptr;
	if (!args[1]->IsNull()) {
		extractRect(isolate, args[1]->ToObject(), &srcRect);
		srcRectPtr = &srcRect;
	}

	UNWRAP(dst, Surface, args[2]);
	SDL_Rect dstRect;
	SDL_Rect *dstRectPtr = nullptr;
	if (!args[3]->IsNull()) {
		extractRect(isolate, args[3]->ToObject(), &dstRect);
		dstRectPtr = &dstRect;
	}

	RETURN(MK_NUMBER(SDL_BlitSurface(src->surface_, srcRectPtr, dst->surface_, dstRectPtr)));
}

METHOD(FillRect) {
    BEGIN();
    UNWRAP(surface, Surface, args[0]);
    SDL_Rect rect;
    extractRect(isolate, args[1]->ToObject(), &rect);
    UINT32ARG(color, 2);
    RETURN(MK_NUMBER(SDL_FillRect(surface->surface_, &rect, color)));
}

void InitSurfaceDrawingFunctions(Local<Object> exports) {
	NODE_SET_METHOD(exports, "blitScaled", BlitScaled);
	NODE_SET_METHOD(exports, "blitSurface", BlitSurface);
    NODE_SET_METHOD(exports, "fillRect", FillRect);
}

}

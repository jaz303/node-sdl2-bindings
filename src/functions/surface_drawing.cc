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

// HELPER
METHOD(BlitScaledCmp) {
	BEGIN();

	UNWRAP(src, Surface, args[0]);
	INTARG(sx, 1);
	INTARG(sy, 2);
	INTARG(sw, 3);
	INTARG(sh, 4);

	UNWRAP(dst, Surface, args[5]);
	INTARG(dx, 6);
	INTARG(dy, 7);
	INTARG(dw, 8);
	INTARG(dh, 9);

	SDL_Rect srcRect = { .x = sx, .y = sy, .w = sw, .h = sh };
	SDL_Rect dstRect = { .x = dx, .y = dy, .w = dw, .h = dh };

	RETURN(MK_NUMBER(SDL_BlitScaled(src->surface_, &srcRect, dst->surface_, &dstRect)));
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

// HELPER
METHOD(BlitSurfaceAtPoint) {
	BEGIN();

	UNWRAP(src, Surface, args[0]);
	UNWRAP(dst, Surface, args[1]);

	INTARG(x, 2);
	INTARG(y, 3);

	SDL_Rect dstRect;
	dstRect.x = x;
	dstRect.y = y;
	dstRect.w = src->surface_->w;
	dstRect.h = src->surface_->h;

	RETURN(MK_NUMBER(SDL_BlitSurface(src->surface_, nullptr, dst->surface_, &dstRect)));
}

METHOD(FillRect) {
    BEGIN();
    UNWRAP(surface, Surface, args[0]);
    SDL_Rect rect;
    SDL_Rect *rectPtr = nullptr;
    if (!args[1]->IsNull()) {
    	extractRect(isolate, args[1]->ToObject(), &rect);
    	rectPtr = &rect;	
    }
	UINT32ARG(color, 2);
    RETURN(MK_NUMBER(SDL_FillRect(surface->surface_, rectPtr, color)));
}

void InitSurfaceDrawingFunctions(Local<Object> exports) {
	NODE_SET_METHOD(exports, "blitScaled", BlitScaled);
	NODE_SET_METHOD(exports, "blitScaledCmp", BlitScaledCmp);
	NODE_SET_METHOD(exports, "blitSurface", BlitSurface);
	NODE_SET_METHOD(exports, "blitSurfaceAtPoint", BlitSurfaceAtPoint);
    NODE_SET_METHOD(exports, "fillRect", FillRect);
}

}

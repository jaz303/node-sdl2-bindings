#include "deps.h"

// Not implemented:
// SDL_ConvertSurface - need PixelFormat implementation
// SDL_CreateRGBSurfaceFrom - need to think about memory management? clone? keep reference?
// SDL_CreateRGBSurfaceWithFormatFrom - ditto
// SDL_LoadBMP_RW
// SDL_SaveBMP_RW
// SDL_SetSurfacePalette - need palette implementation

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

	if (SDL_BlitScaled(src->surface_, srcRectPtr, dst->surface_, dstRectPtr) != 0) {
		THROW_SDL_ERROR();
	}
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

	if (SDL_BlitScaled(src->surface_, &srcRect, dst->surface_, &dstRect) != 0) {
		THROW_SDL_ERROR();
	}
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

	if (SDL_BlitSurface(src->surface_, srcRectPtr, dst->surface_, dstRectPtr) != 0) {
		THROW_SDL_ERROR();
	}
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

	if (SDL_BlitSurface(src->surface_, nullptr, dst->surface_, &dstRect) != 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(ConvertPixels) {
	BEGIN();
	INTARG(width, 0);
	INTARG(height, 1);
	UINT32ARG(srcFormat, 2);
	Local<ArrayBufferView> srcBuffer = Local<ArrayBufferView>::Cast(args[3]);
	void *src = srcBuffer->Buffer()->GetContents().Data();
	INTARG(srcPitch, 4);
	UINT32ARG(dstFormat, 5);
	Local<ArrayBufferView> dstBuffer = Local<ArrayBufferView>::Cast(args[6]);
	void *dst = dstBuffer->Buffer()->GetContents().Data();
	INTARG(dstPitch, 7);
	if (SDL_ConvertPixels(width, height, srcFormat, src, srcPitch, dstFormat, dst, dstPitch) < 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(ConvertSurfaceFormat) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	UINT32ARG(format, 1);
	SDL_Surface *newSurface = SDL_ConvertSurfaceFormat(surface->surface_, format, 0);
	if (newSurface == nullptr) {
		THROW_SDL_ERROR();
	} else {
		RETURN(Surface::NewInstance(isolate, newSurface, true));
	}
}

METHOD(CreateRGBSurface) {
	BEGIN();
	INTARG(width, 0);
	INTARG(height, 1);
	INTARG(depth, 2);
	UINT32ARG(rmask, 3);
	UINT32ARG(gmask, 4);
	UINT32ARG(bmask, 5);
	UINT32ARG(amask, 6);
	auto surface = SDL_CreateRGBSurface(0, width, height, depth, rmask, gmask, bmask, amask);
	if (surface == nullptr) {
		THROW_SDL_ERROR();
	} else {
		RETURN(Surface::NewInstance(isolate, surface, true));
	}
}

METHOD(CreateRGBSurfaceWithFormat) {
	BEGIN();
	INTARG(width, 0);
	INTARG(height, 1);
	INTARG(depth, 2);
	UINT32ARG(format, 3);
	auto surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, depth, format);
	if (surface == nullptr) {
		THROW_SDL_ERROR();
	} else {
		RETURN(Surface::NewInstance(isolate, surface, true));
	}
}



// SDL_CreateRGBSurfaceWithFormat


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
    if (SDL_FillRect(surface->surface_, rectPtr, color) != 0) {
    	THROW_SDL_ERROR();
    }
}

METHOD(FillRects) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	ARRAYARG(rects, 1);
	UINT32ARG(color, 2);
	int count = rects->Length();
	SDL_Rect sdlRects[count];
	for (int ix = 0; ix < count; ++ix) {
		extractRect(isolate, rects->Get(ix)->ToObject(), &sdlRects[ix]);
	}
	if (SDL_FillRects(surface->surface_, sdlRects, count, color) != 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(GetClipRect) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	SDL_Rect out;
	SDL_GetClipRect(surface->surface_, &out);
	auto rect = MK_OBJECT();
	populateRect(isolate, rect, &out);
	RETURN(rect);
}

METHOD(GetColorKey) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	uint32_t colorKey;
	if (SDL_GetColorKey(surface->surface_, &colorKey) < 0) {
		THROW_SDL_ERROR();
	} else {
		RETURN(MK_NUMBER(colorKey));
	}
}

METHOD(GetSurfaceAlphaMod) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	uint8_t alphaMod;
	if (SDL_GetSurfaceAlphaMod(surface->surface_, &alphaMod) < 0) {
		THROW_SDL_ERROR();
	} else {
		RETURN(MK_NUMBER(alphaMod));
	}
}

METHOD(GetSurfaceBlendMode) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	SDL_BlendMode blendMode;
	if (SDL_GetSurfaceBlendMode(surface->surface_, &blendMode) < 0) {
		THROW_SDL_ERROR();
	} else {
		RETURN(MK_NUMBER(blendMode));
	}
}

METHOD(GetSurfaceColorMod) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	uint8_t r, g, b;
	if (SDL_GetSurfaceColorMod(surface->surface_, &r, &g, &b) < 0) {
		THROW_SDL_ERROR();
	} else {
		auto out = MK_OBJECT();
		GET_CONTEXT();
		SET_KEY(out, SYM(r), MK_NUMBER(r));
		SET_KEY(out, SYM(g), MK_NUMBER(g));
		SET_KEY(out, SYM(b), MK_NUMBER(b));
		RETURN(out);
	}
}

METHOD(LoadBMP) {
	BEGIN();
	STRINGARG(file, 0);
	auto surface = SDL_LoadBMP(*file);
	if (surface == nullptr) {
		THROW_SDL_ERROR();
	} else {
		RETURN(Surface::NewInstance(isolate, surface, true));
	}
}

METHOD(LockSurface) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	if (SDL_LockSurface(surface->surface_) < 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(LowerBlit) {
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

	if (SDL_LowerBlit(src->surface_, srcRectPtr, dst->surface_, dstRectPtr) != 0) {
		THROW_SDL_ERROR();
	}
}

// HELPER
METHOD(LowerBlitAtPoint) {
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

	if (SDL_LowerBlit(src->surface_, nullptr, dst->surface_, &dstRect) != 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(LowerBlitScaled) {
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

	if (SDL_LowerBlitScaled(src->surface_, srcRectPtr, dst->surface_, dstRectPtr) != 0) {
		THROW_SDL_ERROR();
	}
}

// HELPER
METHOD(LowerBlitScaledCmp) {
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

	if (SDL_LowerBlitScaled(src->surface_, &srcRect, dst->surface_, &dstRect) != 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(MustLock) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	RETURN(MK_BOOL(SDL_MUSTLOCK(surface->surface_)));
}

METHOD(SaveBMP) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	STRINGARG(file, 1);
	if (SDL_SaveBMP(surface->surface_, *file) < 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(SetClipRect) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	OBJECTARG(rect, 1);
	SDL_Rect sdlRect;
	extractRect(isolate, rect, &sdlRect);
	RETURN(MK_BOOL(SDL_SetClipRect(surface->surface_, &sdlRect)));
}

METHOD(SetColorKey) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	BOOLARG(enable, 1);
	UINT32ARG(color, 2);
	if (SDL_SetColorKey(surface->surface_, enable ? SDL_TRUE : SDL_FALSE, color) < 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(SetSurfaceAlphaMod) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	UINT32ARG(alphaMod, 1);
	if (SDL_SetSurfaceAlphaMod(surface->surface_, alphaMod) < 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(SetSurfaceBlendMode) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	INTARG(blendMode, 1);
	if (SDL_SetSurfaceBlendMode(surface->surface_, (SDL_BlendMode)blendMode) < 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(SetSurfaceColorMod) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	INTARG(r, 1);
	INTARG(g, 2);
	INTARG(b, 3);
	if (SDL_SetSurfaceColorMod(surface->surface_, r, g, b) < 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(SetSurfaceRLE) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	INTARG(flag, 1);
	if (SDL_SetSurfaceRLE(surface->surface_, flag) < 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(UnlockSurface) {
	BEGIN();
	UNWRAP(surface, Surface, args[0]);
	SDL_UnlockSurface(surface->surface_);
}

void InitSurfaceDrawingFunctions(Local<Object> exports) {
	NODE_SET_METHOD(exports, "blitScaled", BlitScaled);
	NODE_SET_METHOD(exports, "blitScaledCmp", BlitScaledCmp);
	NODE_SET_METHOD(exports, "blitSurface", BlitSurface);
	NODE_SET_METHOD(exports, "blitSurfaceAtPoint", BlitSurfaceAtPoint);
	NODE_SET_METHOD(exports, "convertPixels", ConvertPixels);
	NODE_SET_METHOD(exports, "convertSurfaceFormat", ConvertSurfaceFormat);
	NODE_SET_METHOD(exports, "createRGBSurface", CreateRGBSurface);
	NODE_SET_METHOD(exports, "createRGBSurfaceWithFormat", CreateRGBSurfaceWithFormat);
    NODE_SET_METHOD(exports, "fillRect", FillRect);
    NODE_SET_METHOD(exports, "fillRects", FillRect);
    NODE_SET_METHOD(exports, "getClipRect", GetClipRect);
    NODE_SET_METHOD(exports, "getColorKey", GetColorKey);
    NODE_SET_METHOD(exports, "getSurfaceAlphaMod", GetSurfaceAlphaMod);
    NODE_SET_METHOD(exports, "getSurfaceBlendMode", GetSurfaceBlendMode);
    NODE_SET_METHOD(exports, "getSurfaceColorMod", GetSurfaceColorMod);
    NODE_SET_METHOD(exports, "loadBMP", LoadBMP);
    NODE_SET_METHOD(exports, "lockSurface", LockSurface);
    NODE_SET_METHOD(exports, "lowerBlit", LowerBlit);
	NODE_SET_METHOD(exports, "lowerBlitAtPoint", LowerBlitAtPoint);
    NODE_SET_METHOD(exports, "lowerBlitScaled", LowerBlitScaled);
	NODE_SET_METHOD(exports, "lowerBlitScaledCmp", LowerBlitScaledCmp);
	NODE_SET_METHOD(exports, "mustLock", MustLock);
    NODE_SET_METHOD(exports, "saveBMP", SaveBMP);
    NODE_SET_METHOD(exports, "setClipRect", SetClipRect);
    NODE_SET_METHOD(exports, "setColorKey", SetColorKey);
    NODE_SET_METHOD(exports, "setSurfaceAlphaMod", SetSurfaceAlphaMod);
    NODE_SET_METHOD(exports, "setSurfaceBlendMode", SetSurfaceBlendMode);
    NODE_SET_METHOD(exports, "setSurfaceColorMod", SetSurfaceColorMod);
    NODE_SET_METHOD(exports, "setSurfaceRLE", SetSurfaceRLE);
    NODE_SET_METHOD(exports, "unlockSurface", UnlockSurface);
}

}

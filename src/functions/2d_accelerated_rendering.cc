#include "deps.h"

// Not implemented:
// SDL_CreateWindowAndRenderer - just use two separate calls

// Caveats:
// SDL_GL_BindTexture - doesn't return texw, texh
// SDL_GetRenderDrawColor - returns uint32 in ARGB32 format

// TODO:
// SDL_GetRenderer - need to stash renderer on window

// SDL_GetRenderTarget
// SDL_SetRenderTarget
// "set" should stash in hidden field. return it.

// SDL_GetRenderDriverInfo
// SDL_GetRendererInfo
// Just populate an object

// SDL_LockTexture - need to decide best way to get data out
// SDL_UpdateTexture - ditto
// SDL_RenderReadPixels
// SDL_UpdateYUVTexture
// need to sort out pixel data stuff

namespace sdl2_bindings {

using namespace v8;

METHOD(CreateRenderer) {
    BEGIN();
    UNWRAP(window, Window, args[0]);
    INTARG(index, 1);
    UINT32ARG(flags, 2);
    auto renderer = SDL_CreateRenderer(window->window_, index, flags);
    if (renderer == nullptr) {
        THROW_SDL_ERROR();
    } else {
        RETURN(Renderer::NewInstance(isolate, renderer));
    }
}

METHOD(CreateSoftwareRenderer) {
    BEGIN();
    UNWRAP(surface, Surface, args[0]);
    auto renderer = SDL_CreateSoftwareRenderer(surface->surface_);
    if (renderer == nullptr) {
        THROW_SDL_ERROR();
    } else {
        RETURN(Renderer::NewInstance(isolate, renderer));
    }
}

METHOD(CreateTexture) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    UINT32ARG(format, 1);
    INTARG(access, 2);
    INTARG(width, 3);
    INTARG(height, 4);
    auto texture = SDL_CreateTexture(renderer->renderer_, format, access, width, height);
    if (texture == nullptr) {
        THROW_SDL_ERROR();
    } else {
        RETURN(Texture::NewInstance(isolate, texture));
    }
}

METHOD(CreateTextureFromSurface) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    UNWRAP(surface, Surface, args[1]);
    auto texture = SDL_CreateTextureFromSurface(renderer->renderer_, surface->surface_);
    if (texture == nullptr) {
        THROW_SDL_ERROR();
    } else {
        RETURN(Texture::NewInstance(isolate, texture));
    }
}

METHOD(GLBindTexture) {
    BEGIN();
    UNWRAP(texture, Texture, args[0]);
    if (SDL_GL_BindTexture(texture->texture_, nullptr, nullptr) != 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(GLUnbindTexture) {
    BEGIN();
    UNWRAP(texture, Texture, args[0]);
    RETURN(MK_NUMBER(SDL_GL_UnbindTexture(texture->texture_)));
}

METHOD(GetNumRenderDrivers) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetNumRenderDrivers()));
}

METHOD(GetRenderDrawBlendMode) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    SDL_BlendMode blendMode;
    if (SDL_GetRenderDrawBlendMode(renderer->renderer_, &blendMode) < 0) {
        THROW_SDL_ERROR();
    } else {
        RETURN(MK_NUMBER(blendMode));
    }
}

METHOD(GetRenderDrawColor) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    uint8_t r, g, b, a;
    if (SDL_GetRenderDrawColor(renderer->renderer_, &r, &g, &b, &a) < 0) {
        THROW_SDL_ERROR();
    } else {
        uint32_t color = (a << 24) | (r << 16) | (g << 8) | b;
        RETURN(MK_NUMBER(color));
    }
}

METHOD(GetRendererOutputSize) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    int w, h;
    if (SDL_GetRendererOutputSize(renderer->renderer_, &w, &h) < 0) {
        THROW_SDL_ERROR();
    } else {
        auto obj = MK_OBJECT();
        GET_CONTEXT();
        SET_KEY(obj, SYM(width), MK_NUMBER(w));
        SET_KEY(obj, SYM(height), MK_NUMBER(h));
        RETURN(obj);
    }
}

METHOD(GetTextureAlphaMod) {
    BEGIN();
    UNWRAP(texture, Texture, args[0]);
    uint8_t alpha;
    if (SDL_GetTextureAlphaMod(texture->texture_, &alpha) < 0) {
        THROW_SDL_ERROR();
    } else {
        RETURN(MK_NUMBER(alpha));
    }
}

METHOD(GetTextureBlendMode) {
    BEGIN();
    UNWRAP(texture, Texture, args[0]);
    SDL_BlendMode mode;
    if (SDL_GetTextureBlendMode(texture->texture_, &mode) < 0) {
        THROW_SDL_ERROR();
    } else {
        RETURN(MK_NUMBER(mode));
    }
}

METHOD(GetTextureColorMod) {
    BEGIN();
    UNWRAP(texture, Texture, args[0]);
    uint8_t r, g, b;
    if (SDL_GetTextureColorMod(texture->texture_, &r, &g, &b) < 0) {
        THROW_SDL_ERROR();
    } else {
        auto obj = MK_OBJECT();
        GET_CONTEXT();
        SET_KEY(obj, SYM(r), MK_NUMBER(r));
        SET_KEY(obj, SYM(g), MK_NUMBER(g));
        SET_KEY(obj, SYM(b), MK_NUMBER(b));
        RETURN(obj);
    }
}

METHOD(QueryTexture) {
    BEGIN();
    UNWRAP(t, Texture, args[0]);
    uint32_t format;
    int access, w, h;
    if (SDL_QueryTexture(t->texture_, &format, &access, &w, &h) < 0) {
        THROW_SDL_ERROR();
    } else {
        auto out = MK_OBJECT();
        GET_CONTEXT();
        SET_KEY(out, SYM(format), MK_NUMBER(format));
        SET_KEY(out, SYM(access), MK_NUMBER(access));
        SET_KEY(out, SYM(width), MK_NUMBER(w));
        SET_KEY(out, SYM(height), MK_NUMBER(h));
        RETURN(out);
    }
}

METHOD(RenderClear) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    if (SDL_RenderClear(renderer->renderer_) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderCopy) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    UNWRAP(texture, Texture, args[1]);
    SDL_Rect src, dest;
    extractRect(isolate, args[2]->ToObject(), &src);
    extractRect(isolate, args[3]->ToObject(), &dest);
    if (SDL_RenderCopy(renderer->renderer_, texture->texture_, &src, &dest) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderCopyCmp) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    UNWRAP(texture, Texture, args[1]);
	INTARG(sx, 2);
	INTARG(sy, 3);
	INTARG(sw, 4);
	INTARG(sh, 5);
	INTARG(dx, 6);
	INTARG(dy, 7);
	INTARG(dw, 8);
	INTARG(dh, 9);
	SDL_Rect src = { .x = sx, .y = sy, .w = sw, .h = sh };
	SDL_Rect dest = { .x = dx, .y = dy, .w = dw, .h = dh };
	if (SDL_RenderCopy(renderer->renderer_, texture->texture_, &src, &dest) < 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(RenderCopyEx) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    UNWRAP(texture, Texture, args[1]);
    SDL_Rect src, dest;
    extractRect(isolate, args[2]->ToObject(), &src);
    extractRect(isolate, args[3]->ToObject(), &dest);
    DOUBLEARG(angle, 4);
    SDL_Point center;
    SDL_Point *centerPtr = nullptr;
    if (args[5]->IsObject()) {
        extractPoint(isolate, args[5]->ToObject(), &center);
        centerPtr = &center;
    }
    INTARG(flip, 6);
    if (SDL_RenderCopyEx(renderer->renderer_, texture->texture_, &src, &dest, angle, centerPtr, (SDL_RendererFlip)flip) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderCopyExCmp) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    UNWRAP(texture, Texture, args[1]);
	INTARG(sx, 2);
	INTARG(sy, 3);
	INTARG(sw, 4);
	INTARG(sh, 5);
	INTARG(dx, 6);
	INTARG(dy, 7);
	INTARG(dw, 8);
	INTARG(dh, 9);
	SDL_Rect src = { .x = sx, .y = sy, .w = sw, .h = sh };
	SDL_Rect dest = { .x = dx, .y = dy, .w = dw, .h = dh };
    DOUBLEARG(angle, 10);
    SDL_Point center;
    SDL_Point *centerPtr = nullptr;
    if (args[11]->IsObject()) {
        extractPoint(isolate, args[5]->ToObject(), &center);
        centerPtr = &center;
    }
    INTARG(flip, 12);
    if (SDL_RenderCopyEx(renderer->renderer_, texture->texture_, &src, &dest, angle, centerPtr, (SDL_RendererFlip)flip) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderDrawLine) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    INTARG(x1, 1);
    INTARG(y1, 2);
    INTARG(x2, 3);
    INTARG(y2, 4);
    if (SDL_RenderDrawLine(renderer->renderer_, x1, y1, x2, y2) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderDrawLines) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    ARRAYARG(points, 1);
    int count = points->Length();
    SDL_Point sdlPoints[count];
    for (int ix = 0; ix < count; ++ix) {
        extractPoint(isolate, points->Get(ix)->ToObject(), &sdlPoints[ix]);
    }
    if (SDL_RenderDrawLines(renderer->renderer_, sdlPoints, count) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderDrawPoint) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    INTARG(x, 1);
    INTARG(y, 2);
    if (SDL_RenderDrawPoint(renderer->renderer_, x, y) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderDrawPoints) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    ARRAYARG(points, 1);
    int count = points->Length();
    SDL_Point sdlPoints[count];
    for (int ix = 0; ix < count; ++ix) {
        extractPoint(isolate, points->Get(ix)->ToObject(), &sdlPoints[ix]);
    }
    if (SDL_RenderDrawPoints(renderer->renderer_, sdlPoints, count) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderDrawRect) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    OBJECTARG(rect, 1);
    SDL_Rect sdlRect;
    extractRect(isolate, rect, &sdlRect);
    if (SDL_RenderDrawRect(renderer->renderer_, &sdlRect) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderDrawRects) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    ARRAYARG(rects, 1);
    int count = rects->Length();
    SDL_Rect sdlRects[count];
    for (int ix = 0; ix < count; ++ix) {
        extractRect(isolate, rects->Get(ix)->ToObject(), &sdlRects[ix]);
    }
    if (SDL_RenderDrawRects(renderer->renderer_, sdlRects, count) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderFillRect) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    OBJECTARG(rect, 1);
    SDL_Rect sdlRect;
    extractRect(isolate, rect, &sdlRect);
    if (SDL_RenderFillRect(renderer->renderer_, &sdlRect) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderFillRects) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    ARRAYARG(rects, 1);
    int count = rects->Length();
    SDL_Rect sdlRects[count];
    for (int ix = 0; ix < count; ++ix) {
        extractRect(isolate, rects->Get(ix)->ToObject(), &sdlRects[ix]);
    }
    if (SDL_RenderFillRects(renderer->renderer_, sdlRects, count) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderGetClipRect) {
    BEGIN();
    UNWRAP(r, Renderer, args[0]);
    SDL_Rect sdlRect;
    SDL_RenderGetClipRect(r->renderer_, &sdlRect);
    auto obj = MK_OBJECT();
    populateRect(isolate, obj, &sdlRect);
    RETURN(obj);
}

METHOD(RenderGetIntegerScale) {
    BEGIN();
    UNWRAP(r, Renderer, args[0]);
    auto res = SDL_RenderGetIntegerScale(r->renderer_);
    RETURN(MK_BOOL(res == SDL_TRUE));
}

METHOD(RenderGetLogicalSize) {
    BEGIN();
    UNWRAP(r, Renderer, args[0]);
    int w, h;
    SDL_RenderGetLogicalSize(r->renderer_, &w, &h);
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(width), MK_NUMBER(w));
    SET_KEY(out, SYM(height), MK_NUMBER(h));
    RETURN(out);
}

METHOD(RenderGetScale) {
    BEGIN();
    UNWRAP(r, Renderer, args[0]);
    float x, y;
    SDL_RenderGetScale(r->renderer_, &x, &y);
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(x), MK_NUMBER(x));
    SET_KEY(out, SYM(y), MK_NUMBER(y));
    RETURN(out);
}

METHOD(RenderGetViewport) {
    BEGIN();
    UNWRAP(r, Renderer, args[0]);
    SDL_Rect sdlRect;
    SDL_RenderGetViewport(r->renderer_, &sdlRect);
    auto obj = MK_OBJECT();
    populateRect(isolate, obj, &sdlRect);
    RETURN(obj);
}

METHOD(RenderIsClipEnabled) {
    BEGIN();
    UNWRAP(r, Renderer, args[0]);
    auto res = SDL_RenderIsClipEnabled(r->renderer_);
    RETURN(MK_BOOL(res == SDL_TRUE));
}

METHOD(SetRenderDrawBlendMode) {
    BEGIN();
    UNWRAP(r, Renderer, args[0]);
    INTARG(blendMode, 1);
    if (SDL_SetRenderDrawBlendMode(r->renderer_, (SDL_BlendMode)blendMode) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(SetRenderDrawColor) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    INTARG(r, 1);
    INTARG(g, 2);
    INTARG(b, 3);
    INTARG(a, 4);
    if (SDL_SetRenderDrawColor(renderer->renderer_, r, g, b, a) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderPresent) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    SDL_RenderPresent(renderer->renderer_);
}

METHOD(RenderSetClipRect) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    OBJECTARG(rect, 1);
    SDL_Rect sdlRect;
    extractRect(isolate, rect, &sdlRect);
    if (SDL_RenderSetClipRect(renderer->renderer_, &sdlRect) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderSetIntegerScale) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    BOOLARG(enable, 1);
    if (SDL_RenderSetIntegerScale(renderer->renderer_, enable ? SDL_TRUE : SDL_FALSE) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderSetLogicalSize) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    INTARG(w, 1);
    INTARG(h, 2);
    if (SDL_RenderSetLogicalSize(renderer->renderer_, w, h) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderSetScale) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    DOUBLEARG(sx, 1);
    DOUBLEARG(sy, 2);
    if (SDL_RenderSetScale(renderer->renderer_, (float)sx, (float)sy) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderSetViewport) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    OBJECTARG(rect, 1);
    SDL_Rect sdlRect;
    extractRect(isolate, rect, &sdlRect);
    if (SDL_RenderSetViewport(renderer->renderer_, &sdlRect) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(RenderTargetSupported) {
    BEGIN();
    UNWRAP(renderer, Renderer, args[0]);
    RETURN(MK_BOOL(SDL_RenderTargetSupported(renderer->renderer_) == SDL_TRUE));
}

METHOD(SetTextureAlphaMod) {
    BEGIN();
    UNWRAP(t, Texture, args[0]);
    INTARG(alpha, 1);
    if (SDL_SetTextureAlphaMod(t->texture_, alpha) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(SetTextureBlendMode) {
    BEGIN();
    UNWRAP(t, Texture, args[0]);
    INTARG(blendMode, 1);
    if (SDL_SetTextureBlendMode(t->texture_, (SDL_BlendMode)blendMode) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(SetTextureColorMod) {
    BEGIN();
    UNWRAP(t, Texture, args[0]);
    INTARG(r, 1);
    INTARG(g, 2);
    INTARG(b, 3);
    if (SDL_SetTextureColorMod(t->texture_, r, g, b) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(UnlockTexture) {
    BEGIN();
    UNWRAP(texture, Texture, args[0]);
    SDL_UnlockTexture(texture->texture_);
}

void Init2DAcceleratedRenderingFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "createRenderer", CreateRenderer);
    NODE_SET_METHOD(exports, "createSoftwareRenderer", CreateSoftwareRenderer);
    NODE_SET_METHOD(exports, "createTexture", CreateTexture);
    NODE_SET_METHOD(exports, "createTextureFromSurface", CreateTextureFromSurface);
    NODE_SET_METHOD(exports, "glBindTexture", GLBindTexture);
    NODE_SET_METHOD(exports, "glUnbindTexture", GLUnbindTexture);
    NODE_SET_METHOD(exports, "getNumRenderDrivers", GetNumRenderDrivers);
    NODE_SET_METHOD(exports, "getRenderDrawBlendMode", GetRenderDrawBlendMode);
    NODE_SET_METHOD(exports, "getRenderDrawColor", GetRenderDrawColor);
    NODE_SET_METHOD(exports, "getRendererOutputSize", GetRendererOutputSize);
    NODE_SET_METHOD(exports, "getTextureAlphaMod", GetTextureAlphaMod);
    NODE_SET_METHOD(exports, "getTextureBlendMode", GetTextureBlendMode);
    NODE_SET_METHOD(exports, "getTextureColorMod", GetTextureColorMod);
    NODE_SET_METHOD(exports, "queryTexture", QueryTexture);
    NODE_SET_METHOD(exports, "renderClear", RenderClear);
    NODE_SET_METHOD(exports, "renderCopy", RenderCopy);
    NODE_SET_METHOD(exports, "renderCopyCmp", RenderCopyCmp);
    NODE_SET_METHOD(exports, "renderCopyEx", RenderCopy);
    NODE_SET_METHOD(exports, "renderCopyExCmp", RenderCopyExCmp);
    NODE_SET_METHOD(exports, "renderDrawLine", RenderDrawLine);
    NODE_SET_METHOD(exports, "renderDrawLines", RenderDrawLines);
    NODE_SET_METHOD(exports, "renderDrawPoint", RenderDrawPoint);
    NODE_SET_METHOD(exports, "renderDrawPoints", RenderDrawPoints);
    NODE_SET_METHOD(exports, "renderDrawRect", RenderDrawRect);
    NODE_SET_METHOD(exports, "renderDrawRects", RenderDrawRects);
    NODE_SET_METHOD(exports, "renderFillRect", RenderFillRect);
    NODE_SET_METHOD(exports, "renderFillRects", RenderFillRects);
    NODE_SET_METHOD(exports, "renderGetClipRect", RenderGetClipRect);
    NODE_SET_METHOD(exports, "renderGetIntegerScale", RenderGetIntegerScale);
    NODE_SET_METHOD(exports, "renderGetLogicalSize", RenderGetLogicalSize);
    NODE_SET_METHOD(exports, "renderGetScale", RenderGetScale);
    NODE_SET_METHOD(exports, "renderGetViewport", RenderGetViewport);
    NODE_SET_METHOD(exports, "renderIsClipEnabled", RenderIsClipEnabled);
    NODE_SET_METHOD(exports, "renderPresent", RenderPresent);
    NODE_SET_METHOD(exports, "renderSetClipRect", RenderSetClipRect);
    NODE_SET_METHOD(exports, "renderSetIntegerScale", RenderSetIntegerScale);
    NODE_SET_METHOD(exports, "renderSetLogicalSize", RenderSetLogicalSize);
    NODE_SET_METHOD(exports, "renderSetScale", RenderSetScale);
    NODE_SET_METHOD(exports, "renderSetViewport", RenderSetViewport);
    NODE_SET_METHOD(exports, "renderTargetSupported", RenderTargetSupported);
    NODE_SET_METHOD(exports, "setRenderDrawBlendMode", SetRenderDrawBlendMode);
    NODE_SET_METHOD(exports, "setRenderDrawColor", SetRenderDrawColor);
    NODE_SET_METHOD(exports, "setTextureAlphaMod", SetTextureAlphaMod);
    NODE_SET_METHOD(exports, "setTextureBlendMode", SetTextureBlendMode);
    NODE_SET_METHOD(exports, "setTextureColorMod", SetTextureColorMod);
    NODE_SET_METHOD(exports, "unlockTexture", UnlockTexture);
}

}

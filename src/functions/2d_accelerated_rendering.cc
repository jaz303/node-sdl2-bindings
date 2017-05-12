#include "deps.h"

// Not implemented:
// SDL_CreateWindowAndRenderer - just use two separate calls
// SDL_GetRenderTarget - need to be able to map from SDL_Texture* to Texture
// SDL_GetRenderer - need to stash renderer on window

// Caveats:
// SDL_GL_BindTexture - doesn't return texw, texh
// SDL_GetRenderDrawColor - returns uint32 in ARGB32 format

// TODO:
// SDL_GetRenderDriverInfo
// SDL_GetRendererInfo

// SDL_LockTexture
// SDL_QueryTexture
// SDL_RenderClear
// SDL_RenderCopy
// SDL_RenderCopyEx
// SDL_RenderDrawLine
// SDL_RenderDrawLines
// SDL_RenderDrawPoint
// SDL_RenderDrawPoints
// SDL_RenderDrawRect
// SDL_RenderDrawRects
// SDL_RenderFillRect
// SDL_RenderFillRects
// SDL_RenderGetClipRect
// SDL_RenderGetIntegerScale
// SDL_RenderGetLogicalSize
// SDL_RenderGetScale
// SDL_RenderGetViewport
// SDL_RenderIsClipEnabled
// SDL_RenderPresent
// SDL_RenderReadPixels
// SDL_RenderSetClipRect
// SDL_RenderSetIntegerScale
// SDL_RenderSetLogicalSize
// SDL_RenderSetScale
// SDL_RenderSetViewport
// SDL_RenderTargetSupported
// SDL_SetRenderDrawBlendMode
// SDL_SetRenderDrawColor
// SDL_SetRenderTarget
// SDL_SetTextureAlphaMod
// SDL_SetTextureBlendMode
// SDL_SetTextureColorMod
// SDL_UnlockTexture
// SDL_UpdateTexture
// SDL_UpdateYUVTexture

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
}

}

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

#include "_gl.cc"

#include "_helpers.cc"
#include "_display.cc"
#include "_gl_context.cc"
#include "_window.cc"



//
// Surface

v8::Persistent<v8::Function> Surface::constructor;

Surface::Surface(SDL_Surface *surface, bool owned) : surface_(surface), owned_(owned), cairoContext_(nullptr) {}
Surface::~Surface() {
    if (owned_) {
        SDL_FreeSurface(surface_);
    }
    if (cairoContext_) {
        cairo_destroy(cairoContext_);
    }
}

void Surface::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Surface"));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
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

    DEFINE_METHOD(tpl, "getContext", GetContext);

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

// FIXME: error/return value checking from cairo functions
METHOD(Surface::GetContext) {
    BEGIN();
    UNWRAP_ME(self, Surface);
    auto existing = args.This()->GetInternalField(1);
    if (existing->IsUndefined()) {
        auto cairoSurface = cairo_image_surface_create_for_data(
            (unsigned char*)self->surface_->pixels,
            CAIRO_FORMAT_ARGB32,
            self->surface_->w,
            self->surface_->h,
            self->surface_->pitch
        );
        auto cairoContext = cairo_create(cairoSurface);
        cairo_surface_destroy(cairoSurface);
        auto ctx = Context2D::NewInstance(isolate, cairoContext);
        cairo_destroy(cairoContext);
        args.This()->SetInternalField(1, ctx);
        RETURN(ctx);
    } else {
        RETURN(existing);
    }
}

//
// Renderer

v8::Persistent<v8::Function> Renderer::constructor;

Renderer::Renderer(SDL_Renderer *renderer) : renderer_(renderer) {}
Renderer::~Renderer() { SDL_DestroyRenderer(renderer_); }

void Renderer::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Renderer"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    DEFINE_ACCESSOR(tpl, "drawColor", GetDrawColor, SetDrawColor);
    DEFINE_ACCESSOR(tpl, "blendMode", GetBlendMode, SetBlendMode);
    DEFINE_METHOD(tpl, "createTexture", CreateTexture);
    DEFINE_METHOD(tpl, "createTextureFromSurface", CreateTextureFromSurface);
    DEFINE_METHOD(tpl, "clear", Clear);
    DEFINE_METHOD(tpl, "copy", Copy);
    DEFINE_METHOD(tpl, "copyRect", CopyRect);
    DEFINE_METHOD(tpl, "copyEx", CopyEx);
    DEFINE_METHOD(tpl, "copyRectEx", CopyRectEx);
    DEFINE_METHOD(tpl, "drawLine", DrawLine);
    DEFINE_METHOD(tpl, "drawPoint", DrawPoint);
    DEFINE_METHOD(tpl, "drawRect", DrawRect);
    DEFINE_METHOD(tpl, "fillRect", FillRect);
    DEFINE_METHOD(tpl, "present", Present);
    DEFINE_METHOD(tpl, "setRenderTarget", SetRenderTarget);
    DEFINE_METHOD(tpl, "setDefaultRenderTarget", SetDefaultRenderTarget);
    constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> Renderer::NewInstance(Isolate *isolate, SDL_Renderer *renderer) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    Renderer *r = new Renderer(renderer);
    r->Wrap(instance);
    return instance;
}

METHOD(Renderer::New) {}

GETTER(Renderer::GetDrawColor) {
    BEGIN();
    UNWRAP_ME(self, Renderer);
    uint8_t a, r, g, b;
    if (SDL_GetRenderDrawColor(self->renderer_, &r, &g, &b, &a) != 0) {
        THROW_SDL_ERROR();
    }
    uint32_t argb = (a << 24) | (r << 16) | (g << 8) | b;
    RETURN(MK_NUMBER(argb));
}

SETTER(Renderer::SetDrawColor) {
    BEGIN();
    UNWRAP_ME(self, Renderer);
    uint32_t argb = value->Uint32Value();
    uint8_t a = argb >> 24;
    uint8_t r = argb >> 16;
    uint8_t g = argb >> 8;
    uint8_t b = argb;
    if (SDL_SetRenderDrawColor(self->renderer_, r, g, b, a) != 0) {
        THROW_SDL_ERROR();
    }
}

GETTER(Renderer::GetBlendMode) {
    BEGIN();
    UNWRAP_ME(self, Renderer);
    SDL_BlendMode out;
    if (SDL_GetRenderDrawBlendMode(self->renderer_, &out) != 0) {
        THROW_SDL_ERROR();
    }
    RETURN(MK_NUMBER(out));
}

SETTER(Renderer::SetBlendMode) {
    BEGIN();
    UNWRAP_ME(self, Renderer); 
    if (SDL_SetRenderDrawBlendMode(self->renderer_, (SDL_BlendMode)value->Int32Value()) != 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(Renderer::CreateTexture) {
    BEGIN();
    UNWRAP_ME(r, Renderer);
    NARGS(4);
    UINT32ARG(format, 0);
    INTARG(flags, 1);
    INTARG(width, 2);
    INTARG(height, 3);
    SDL_Texture *texture = SDL_CreateTexture(r->renderer_, format, flags, width, height);
    if (texture == NULL) {
        THROW_SDL_ERROR();
    }
    RETURN(Texture::NewInstance(isolate, texture));
}

METHOD(Renderer::CreateTextureFromSurface) {
    BEGIN();
    UNWRAP_ME(r, Renderer);
    UNWRAP(s, Surface, args[0]);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(r->renderer_, s->surface_);
    if (texture == NULL) {
        THROW_SDL_ERROR();
    }
    RETURN(Texture::NewInstance(isolate, texture));
}

METHOD(Renderer::Clear) {
    UNWRAP_ME(r, Renderer);
    SDL_RenderClear(r->renderer_);
}

// 0 - cover target with texture
// 4 - copy texture to target rect
// 8 - copy texture src rect to target rect
METHOD(Renderer::Copy) {
    BEGIN();
    UNWRAP_ME(self, Renderer);
    UNWRAP(texture, Texture, args[0]);
    SDL_Rect src, dest;
    if (args.Length() == 1) {
        SDL_RenderCopy(self->renderer_, texture->texture_, NULL, NULL);
    } else if (args.Length() == 5) {
        dest.x = args[1]->Int32Value();
        dest.y = args[2]->Int32Value();
        dest.w = args[3]->Int32Value();
        dest.h = args[4]->Int32Value();
        SDL_RenderCopy(self->renderer_, texture->texture_, NULL, &dest);
    } else if (args.Length() == 9) {
        src.x = args[1]->Int32Value();
        src.y = args[2]->Int32Value();
        src.w = args[3]->Int32Value();
        src.h = args[4]->Int32Value();
        dest.x = args[5]->Int32Value();
        dest.y = args[6]->Int32Value();
        dest.w = args[7]->Int32Value();
        dest.h = args[8]->Int32Value();
        SDL_RenderCopy(self->renderer_, texture->texture_, &src, &dest);
    } else {
        THROW(Error, "argument error");
    }
}

// 9 - texture, dest-rect, rotation, cx, cy, flags
// 13 - texture, src-rect, dest-rect, rotation, cx, cy, flags
METHOD(Renderer::CopyEx) {
    BEGIN();
    UNWRAP_ME(self, Renderer);
    UNWRAP(texture, Texture, args[0]);
    SDL_Rect src, dest;
    SDL_Rect *srcp;
    SDL_Point center;
    SDL_Point *centerp = &center;
    int base;
    if (args.Length() == 9) {
        srcp = NULL;
        base = 1;
    } else if (args.Length() == 13) {
        srcp = &src;
        base = 5;
        src.x = args[1]->Int32Value();
        src.y = args[2]->Int32Value();
        src.w = args[3]->Int32Value();
        src.h = args[4]->Int32Value();
    } else {
        THROW(TypeError, "argument error");
    }
    dest.x = args[base+0]->Int32Value();
    dest.y = args[base+1]->Int32Value();
    dest.w = args[base+2]->Int32Value();
    dest.h = args[base+3]->Int32Value();
    double rotation = args[base+4]->NumberValue();
    center.x = args[base+5]->Int32Value();
    center.y = args[base+6]->Int32Value();
    if (center.x == -1) {
        centerp = NULL;
    }
    SDL_RendererFlip flip = (SDL_RendererFlip)args[base+7]->Int32Value();
    SDL_RenderCopyEx(self->renderer_, texture->texture_, srcp, &dest, rotation, centerp, flip);
}

METHOD(Renderer::CopyRect) {
    BEGIN();
    NARGS(3);
    UNWRAP_ME(self, Renderer);
    UNWRAP(texture, Texture, args[0]);
    SDL_Rect src, dest;
    SDL_Rect *srcp = NULL, *destp = NULL;
    if (args[1]->IsObject()) {
        extractRect(isolate, args[1]->ToObject(), &src);
        srcp = &src;
    }
    if (args[2]->IsObject()) {
        extractRect(isolate, args[2]->ToObject(), &dest);
        destp = &dest;
    }
    SDL_RenderCopy(self->renderer_, texture->texture_, srcp, destp);
}

// 7 - texture, src-rect, dest-rect, rotation, cx, cy, flags
METHOD(Renderer::CopyRectEx) {
    BEGIN();
    NARGS(7);
    UNWRAP_ME(self, Renderer);
    UNWRAP(texture, Texture, args[0]);
    SDL_Rect src, dest;
    SDL_Rect *srcp = NULL, *destp = NULL;
    if (args[1]->IsObject()) {
        extractRect(isolate, args[1]->ToObject(), &src);
        srcp = &src;
    }
    if (args[2]->IsObject()) {
        extractRect(isolate, args[2]->ToObject(), &dest);
        destp = &dest;
    }
    SDL_Point center;
    SDL_Point *centerp = &center;
    double rotation = args[3]->NumberValue();
    center.x = args[4]->Int32Value();
    center.y = args[5]->Int32Value();
    if (center.x == -1) {
        centerp = NULL;
    }
    SDL_RendererFlip flip = (SDL_RendererFlip)args[6]->Int32Value();
    SDL_RenderCopyEx(self->renderer_, texture->texture_, srcp, destp, rotation, centerp, flip);
}

METHOD(Renderer::DrawLine) {
    BEGIN();
    NARGS(4);
    INTARG(x1, 0);
    INTARG(y1, 1);
    INTARG(x2, 2);
    INTARG(y2, 3);
    UNWRAP_ME(self, Renderer);
    SDL_RenderDrawLine(self->renderer_, x1, y1, x2, y2);
}

METHOD(Renderer::DrawPoint) {
    BEGIN();
    NARGS(2);
    INTARG(x, 0);
    INTARG(y, 1);
    UNWRAP_ME(self, Renderer);
    SDL_RenderDrawPoint(self->renderer_, x, y);
}

void drawOrFillRect(SDL_Renderer *renderer, const FunctionCallbackInfo<Value>& args, bool fill) {
    Isolate *isolate = args.GetIsolate();
    SDL_Rect rect;
    if (args.Length() == 1) {
        extractRect(isolate, args[0]->ToObject(), &rect);
    } else if (args.Length() == 4) {
        rect.x = args[0]->Int32Value();
        rect.y = args[1]->Int32Value();
        rect.w = args[2]->Int32Value();
        rect.h = args[3]->Int32Value();
    } else {
        THROW(TypeError, "argument error");
    }
    if (fill) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderDrawRect(renderer, &rect);
    }
}

METHOD(Renderer::DrawRect) {
    UNWRAP_ME(self, Renderer);
    drawOrFillRect(self->renderer_, args, false);
}

METHOD(Renderer::FillRect) {
    UNWRAP_ME(self, Renderer);
    drawOrFillRect(self->renderer_, args, true);
}

METHOD(Renderer::Present) {
    UNWRAP_ME(r, Renderer);
    SDL_RenderPresent(r->renderer_);
}

METHOD(Renderer::SetRenderTarget) {
    BEGIN();
    UNWRAP_ME(r, Renderer);
    UNWRAP(t, Texture, args[0]);
    if (SDL_SetRenderTarget(r->renderer_, t->texture_) != 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(Renderer::SetDefaultRenderTarget) {
    BEGIN();
    UNWRAP_ME(r, Renderer);
    if (SDL_SetRenderTarget(r->renderer_, NULL) != 0) {
        THROW_SDL_ERROR();
    }
}

//
// Texture

v8::Persistent<v8::Function> Texture::constructor;

Texture::Texture(SDL_Texture *texture) : texture_(texture) {}
Texture::~Texture() { SDL_DestroyTexture(texture_); }

void Texture::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Texture"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    DEFINE_METHOD(tpl, "bind", Bind);
    DEFINE_METHOD(tpl, "unbind", Unbind);
    DEFINE_GETTER(tpl, "width", GetWidth);
    DEFINE_GETTER(tpl, "height", GetHeight);
    DEFINE_GETTER(tpl, "format", GetFormat);
    DEFINE_GETTER(tpl, "access", GetAccess);
    constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> Texture::NewInstance(Isolate *isolate, SDL_Texture *texture) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    Texture *t = new Texture(texture);
    t->Wrap(instance);
    return instance;
}

METHOD(Texture::New) {}

METHOD(Texture::Bind) {
    BEGIN();
    UNWRAP_ME(self, Texture);
    float width, height;
    if (SDL_GL_BindTexture(self->texture_, &width, &height) != 0) {
        THROW_SDL_ERROR();
    }
    if (args.Length() > 0) {
        auto obj = args[0]->ToObject();
        auto ctx = isolate->GetCurrentContext();
        obj->CreateDataProperty(ctx, SYM(width), MK_NUMBER(width));
        obj->CreateDataProperty(ctx, SYM(height), MK_NUMBER(height));
    }
}

METHOD(Texture::Unbind) {
    BEGIN();
    UNWRAP_ME(self, Texture);
    if (SDL_GL_UnbindTexture(self->texture_) != 0) {
        THROW_SDL_ERROR();
    }
}

GETTER(Texture::GetWidth) {
    BEGIN();
    UNWRAP_ME(t, Texture);
    int out;
    SDL_QueryTexture(t->texture_, NULL, NULL, &out, NULL);
    RETURN(MK_NUMBER(out));
}

GETTER(Texture::GetHeight) {
    BEGIN();
    UNWRAP_ME(t, Texture);
    int out;
    SDL_QueryTexture(t->texture_, NULL, NULL, NULL, &out);
    RETURN(MK_NUMBER(out)); 
}

GETTER(Texture::GetFormat) {
    BEGIN();
    UNWRAP_ME(t, Texture);
    uint32_t out;
    SDL_QueryTexture(t->texture_, &out, NULL, NULL, NULL);
    RETURN(MK_NUMBER(out));
}

GETTER(Texture::GetAccess) {
    BEGIN();
    UNWRAP_ME(t, Texture);
    int out;
    SDL_QueryTexture(t->texture_, NULL, &out, NULL, NULL);
    RETURN(MK_NUMBER(out));
}

//
// Context2D

// TODO: this class should hold a reference to the surface in an internal
// field so that the surface will never be destroyed for as long as there's
// an active reference to the context. Also provide a getter for it.

v8::Persistent<v8::Function> Context2D::constructor;

Context2D::Context2D(cairo_t *ctx) : ctx_(ctx) { cairo_reference(ctx_); }
Context2D::~Context2D() { cairo_destroy(ctx_); }

void Context2D::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Context2D"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    DEFINE_METHOD(tpl, "save", Save);
    DEFINE_METHOD(tpl, "restore", Restore);
    // DEFINE_METHOD(tpl, "bind", Bind);
    constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> Context2D::NewInstance(Isolate *isolate, cairo_t *cctx) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    Context2D *c = new Context2D(cctx);
    c->Wrap(instance);
    return instance;
}

METHOD(Context2D::New) {}

METHOD(Context2D::Save) {
    UNWRAP_ME(self, Context2D);
    cairo_save(self->ctx_);
}

METHOD(Context2D::Restore) {
    UNWRAP_ME(self, Context2D);
    cairo_restore(self->ctx_);
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

//
// Events

#include "events.cc"

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
//  // TODO
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
//  BEGIN();
//  NARGS(1);
//  INTARG(deviceIndex, 0);
//  RETURN(MK_NUMBER(SDL_JoystickGetDeviceGUID(deviceIndex)));  
// }

// METHOD(JoystickGetGUID) {
//  // TODO
// }

// METHOD(JoystickGetGUIDFromString) {
//  // TODO
// }

// METHOD(JoystickGetGUIDString) {
//  // TODO
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
// Clipboard

METHOD(GetClipboardText) {
    BEGIN();
    const char *text = SDL_GetClipboardText();
    if (text == NULL) {
        THROW_SDL_ERROR();
    } else {
        RETURN(MK_STRING(text));
    }
}

METHOD(HasClipboardText) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasClipboardText() == SDL_TRUE));
}

METHOD(SetClipboardText) {
    BEGIN();
    String::Utf8Value text(args[0]->ToString());
    int res = SDL_SetClipboardText(*text);
    if (res != 0) {
        THROW_SDL_ERROR();
    }
}

//
// Timing

METHOD(Delay) {
    BEGIN();
    UINT32ARG(ms, 0);
    SDL_Delay(ms);
}

METHOD(GetTicks) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetTicks()));
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
// Screensaver



//
// Video




//
//

void SDL2ModuleInit(Local<Object> exports) {
    Isolate *isolate = exports->GetIsolate();

    #define DEFSYM(name) sym_##name.Reset(isolate, String::NewFromUtf8(isolate, #name));
    #include "symbols.x"
    #undef DEFSYM

    initConstants(exports);
    initGL(exports);

    Window::Init(isolate);
    Surface::Init(isolate);
    Renderer::Init(isolate);
    Texture::Init(isolate);
    Context2D::Init(isolate);
    GLContext::Init(isolate);

    NODE_SET_METHOD(exports, "init", Init);

    initEvents(exports);
    initDisplay(exports);

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

    // Clipboard
    NODE_SET_METHOD(exports, "getClipboardText", GetClipboardText);
    NODE_SET_METHOD(exports, "hasClipboardText", HasClipboardText);
    NODE_SET_METHOD(exports, "setClipboardText", SetClipboardText);

    // Timings
    NODE_SET_METHOD(exports, "delay", Delay);
    NODE_SET_METHOD(exports, "getTicks", GetTicks);

    // Images
    NODE_SET_METHOD(exports, "imageInit", ImageInit);
    NODE_SET_METHOD(exports, "imageQuit", ImageQuit);
    NODE_SET_METHOD(exports, "imageLoad", ImageLoad);

}

NODE_MODULE(sdl2, SDL2ModuleInit);

}
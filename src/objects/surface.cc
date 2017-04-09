#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

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
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    DEFINE_GETTER(tpl, "width", GetWidth);
    DEFINE_GETTER(tpl, "height", GetHeight);
    DEFINE_GETTER(tpl, "pitch", GetPitch);
    DEFINE_GETTER(tpl, "stride", GetPitch);
    DEFINE_GETTER(tpl, "data", GetData);
    DEFINE_GETTER(tpl, "format", GetFormat);
    constructor.Reset(isolate, tpl->GetFunction());
    // DEFINE_METHOD(tpl, "blit", Blit);
    // DEFINE_METHOD(tpl, "blitRect", BlitRect);
    // DEFINE_METHOD(tpl, "blitRectScaled", BlitRectScaled);
    // DEFINE_METHOD(tpl, "blitScaled", BlitScaled);
    // DEFINE_METHOD(tpl, "clear", Clear);
    // DEFINE_METHOD(tpl, "fillRect", FillRect);
    // DEFINE_METHOD(tpl, "mapRGB", MapRGB);
    // DEFINE_METHOD(tpl, "mapRGBA", MapRGBA);
    // DEFINE_METHOD(tpl, "getContext", GetContext);
}

Local<Object> Surface::NewInstance(Isolate *isolate, SDL_Surface *surface, bool owned) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    Surface *s = new Surface(surface, owned);
    s->Wrap(instance);
    return instance;
}

METHOD(Surface::New) {}

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

GETTER(Surface::GetData) {
    BEGIN();
    UNWRAP(s, Surface, args.This());
    auto existingData = args.This()->GetInternalField(1);
    if (existingData->IsUndefined()) {
        auto buffer = ArrayBuffer::New(isolate, s->surface_->pixels, s->surface_->pitch * s->surface_->h);
        args.This()->SetInternalField(1, buffer);
        RETURN(buffer);
    } else {
        RETURN(existingData);
    }
}

GETTER(Surface::GetFormat) {
    BEGIN();
    UNWRAP(s, Surface, args.This());
    RETURN(MK_NUMBER(s->surface_->format->format));
}

// // FIXME: error/return value checking from cairo functions
// METHOD(Surface::GetContext) {
//     BEGIN();
//     UNWRAP_ME(self, Surface);
//     auto existing = args.This()->GetInternalField(1);
//     if (existing->IsUndefined()) {
//         auto cairoSurface = cairo_image_surface_create_for_data(
//             (unsigned char*)self->surface_->pixels,
//             CAIRO_FORMAT_ARGB32,
//             self->surface_->w,
//             self->surface_->h,
//             self->surface_->pitch
//         );
//         auto cairoContext = cairo_create(cairoSurface);
//         cairo_surface_destroy(cairoSurface);
//         auto ctx = Context2D::NewInstance(isolate, cairoContext);
//         cairo_destroy(cairoContext);
//         args.This()->SetInternalField(1, ctx);
//         RETURN(ctx);
//     } else {
//         RETURN(existing);
//     }
// }

// // blit(src, x, y)
// // blit(src, sx, sy, sw, sh, x, y)
// METHOD(Surface::Blit) {
//     BEGIN();

//     UNWRAP_ME(dst, Surface);
//     UNWRAP(src, Surface, args[0]);

//     SDL_Surface *ds = dst->surface_, *ss = src->surface_;
//     SDL_Rect dr, sr;

//     switch (args.Length()) {
//         case 3:
//         {
//             dr.x = args[1]->Int32Value();
//             dr.y = args[2]->Int32Value();
//             SDL_BlitSurface(ss, NULL, ds, &dr);
//             break;
//         }
//         case 7:
//         {
//             sr.x = args[1]->Int32Value();
//             sr.y = args[2]->Int32Value();
//             sr.w = args[3]->Int32Value();
//             sr.h = args[4]->Int32Value();
//             dr.x = args[5]->Int32Value();
//             dr.y = args[6]->Int32Value();
//             SDL_BlitSurface(ss, &sr, ds, &dr);
//             break;
//         }
//         default:
//         {
//             THROW(TypeError, "argument error");
//         }
//     }
// }

// // blitRect(src, srcRect, destRect)
// // blitRect(src, srcRect, x, y)
// METHOD(Surface::BlitRect) {
//     BEGIN();
//     THROW(Error, "not implemented");
// }

// // blitRectScaled(src, srcRect, destRect)
// // blitRectScaled(src, srcRect, x, y)
// // blitRectScaled(src, srcRect, x, y, w, h)
// METHOD(Surface::BlitRectScaled) {
//     BEGIN();
//     THROW(Error, "not implemented");
// }

// // 1 blitScaled(src) - fill destination with src
// // 5 blitScaled(src, x, y, w, h) - blit src into x,y,w,h
// // 9 blitScaled(src, sx, sy, sw, sh, x, y, w, h)
// METHOD(Surface::BlitScaled) {
//     BEGIN();

//     UNWRAP_ME(dst, Surface);
//     UNWRAP(src, Surface, args[0]);

//     SDL_Surface *ds = dst->surface_, *ss = src->surface_;
//     SDL_Rect dr, sr;

//     switch (args.Length()) {
//         case 1:
//         {
//             SDL_BlitScaled(ss, NULL, ds, NULL);
//             break;
//         }
//         case 5:
//         {
//             dr.x = args[1]->Int32Value();
//             dr.y = args[2]->Int32Value();
//             dr.w = args[3]->Int32Value();
//             dr.h = args[4]->Int32Value();
//             SDL_BlitScaled(ss, NULL, ds, &dr);
//             break;
//         }
//         case 9:
//         {
//             sr.x = args[1]->Int32Value();
//             sr.y = args[2]->Int32Value();
//             sr.w = args[3]->Int32Value();
//             sr.h = args[4]->Int32Value();
//             dr.x = args[5]->Int32Value();
//             dr.y = args[6]->Int32Value();
//             dr.w = args[7]->Int32Value();
//             dr.h = args[8]->Int32Value();
//             SDL_BlitScaled(ss, &sr, ds, &dr);
//             break;
//         }
//         default:
//         {
//             THROW(TypeError, "argument error");
//         }
//     }
// }

// METHOD(Surface::Clear) {
//     BEGIN();
//     NARGS(1);
//     UINT32ARG(color, 0);
//     UNWRAP_ME(s, Surface);
//     SDL_FillRect(s->surface_, NULL, color);
// }

// METHOD(Surface::FillRect) {
//     BEGIN();
//     UNWRAP_ME(s, Surface);
//     uint32_t color;
//     SDL_Rect r;
//     if (args.Length() == 2) {
//         // FIXME: typecheck needed?
//         extractRect(isolate, args[0]->ToObject(), &r);
//         color = args[1]->Uint32Value();
//     } else if (args.Length() == 5) {
//         r.x = args[0]->Int32Value();
//         r.y = args[1]->Int32Value();
//         r.w = args[2]->Int32Value();
//         r.h = args[3]->Int32Value();
//         color = args[4]->Uint32Value();
//     } else {
//         THROW(TypeError, "argument error");
//     }
//     SDL_FillRect(s->surface_, &r, color);
// }

// METHOD(Surface::MapRGB) {
//     BEGIN();
//     NARGS(3);
//     INTARG(r, 0);
//     INTARG(g, 1);
//     INTARG(b, 2);
//     UNWRAP_ME(s, Surface);
//     RETURN(MK_NUMBER(SDL_MapRGB(s->surface_->format, r, g, b)));
// }

// METHOD(Surface::MapRGBA) {
//     BEGIN();
//     NARGS(4);
//     INTARG(r, 0);
//     INTARG(g, 1);
//     INTARG(b, 2);
//     INTARG(a, 3);
//     UNWRAP_ME(s, Surface);
//     RETURN(MK_NUMBER(SDL_MapRGBA(s->surface_->format, r, g, b, a)));
// }

}
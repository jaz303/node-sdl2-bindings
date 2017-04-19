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

}
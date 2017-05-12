#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

v8::Persistent<v8::Function> Texture::constructor;

Texture::Texture(SDL_Texture *texture) : texture_(texture) {}
Texture::~Texture() { destroy(); }

void Texture::destroy() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

void Texture::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Texture"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
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

}
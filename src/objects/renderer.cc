#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

v8::Persistent<v8::Function> Renderer::constructor;

Renderer::Renderer(SDL_Renderer *renderer) : renderer_(renderer) {}
Renderer::~Renderer() { destroy(); }

void Renderer::destroy() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
}

void Renderer::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Renderer"));
    tpl->InstanceTemplate()->SetInternalFieldCount(3);
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

}
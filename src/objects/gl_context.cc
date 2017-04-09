#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

v8::Persistent<v8::Function> GLContext::constructor;

GLContext::GLContext(SDL_GLContext ctx) : ctx_(ctx), destroyed_(false) {}
GLContext::~GLContext() { destroy(); }

void GLContext::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "GLContext"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> GLContext::NewInstance(Isolate *isolate, SDL_GLContext ctx) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    GLContext *c = new GLContext(ctx);
    c->Wrap(instance);
    return instance;
}

void GLContext::destroy() {
    if (!destroyed_) {
        SDL_GL_DeleteContext(ctx_);
        destroyed_ = true;
    }
}

bool GLContext::isDestroyed() {
    return destroyed_;
}

METHOD(GLContext::New) {}

}

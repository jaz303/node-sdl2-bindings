#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

v8::Persistent<v8::Function> Cursor::constructor;

Cursor::Cursor(SDL_Cursor *cursor, bool userCreated) : cursor_(cursor), userCreated_(userCreated), destroyed_(false) {}
Cursor::~Cursor() { destroy(); }

void Cursor::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Cursor"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> Cursor::NewInstance(Isolate *isolate, SDL_Cursor *cursor, bool userCreated) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    Cursor *c = new Cursor(cursor, userCreated);
    c->Wrap(instance);
    return instance;
}

void Cursor::destroy() {
    if (userCreated_ && !destroyed_) {
        SDL_FreeCursor(cursor_);
        destroyed_ = true;
    }
}

bool Cursor::isDestroyed() {
    return destroyed_;
}

METHOD(Cursor::New) {}

}

#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

v8::Persistent<v8::Function> Joystick::constructor;

Joystick::Joystick(SDL_Joystick *joystick) : joystick_(joystick), closed_(false) {}
Joystick::~Joystick() { close(); }

void Joystick::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Joystick"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> Joystick::NewInstance(Isolate *isolate, SDL_Joystick *joystick) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    Joystick *c = new Joystick(joystick);
    c->Wrap(instance);
    return instance;
}

void Joystick::close() {
    if (!closed_) {
        SDL_JoystickClose(joystick_);
        closed_ = true;
    }
}

bool Joystick::isClosed() {
    return closed_;
}

METHOD(Joystick::New) {}

}

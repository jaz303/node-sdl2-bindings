#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

v8::Persistent<v8::Function> Window::constructor;

Window::Window(SDL_Window *window) : window_(window) {}
Window::~Window() { destroy(); }

void Window::destroy() {
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
}

void Window::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Window"));
    tpl->InstanceTemplate()->SetInternalFieldCount(3);
    constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> Window::NewInstance(Isolate *isolate, SDL_Window *window) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    Window *w = new Window(window);
    w->Wrap(instance);
    attachWindowReference(window, w);
    return instance;
}

METHOD(Window::New) {}

}
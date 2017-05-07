#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

v8::Persistent<v8::Function> GameController::constructor;

GameController::GameController(SDL_GameController *gameController) : gameController_(gameController), closed_(false) {}
GameController::~GameController() { close(); }

void GameController::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "GameController"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> GameController::NewInstance(Isolate *isolate, SDL_GameController *gameController) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    GameController *c = new GameController(gameController);
    c->Wrap(instance);
    return instance;
}

void GameController::close() {
    if (!closed_) {
        SDL_GameControllerClose(gameController_);
        closed_ = true;
    }
}

bool GameController::isClosed() {
    return closed_;
}

METHOD(GameController::New) {}

}

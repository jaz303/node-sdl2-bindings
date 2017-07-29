#include "deps.h"

#include <stdio.h>

namespace sdl2_bindings {

using namespace v8;

v8::Persistent<v8::Function> AudioDevice::constructor;

AudioDevice::AudioDevice(SDL_AudioDeviceID deviceId, audio_context *ctx) : deviceId_(deviceId), audioContext_(ctx), destroyed_(false) {}
AudioDevice::~AudioDevice() { destroy(); }

void AudioDevice::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "AudioDevice"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> AudioDevice::NewInstance(Isolate *isolate, SDL_AudioDeviceID deviceId, audio_context *ctx) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    AudioDevice *c = new AudioDevice(deviceId, ctx);
    c->Wrap(instance);
    return instance;
}

void AudioDevice::destroy() {
    if (!destroyed_) {
        SDL_CloseAudioDevice(deviceId_);
        delete audioContext_;
        deviceId_ = -1;
        audioContext_ = nullptr;
        destroyed_ = true;
    }
}

bool AudioDevice::isDestroyed() {
    return destroyed_;
}

METHOD(AudioDevice::New) {}

}

#include "deps.h"
#include <node.h>
#include <uv.h>

// TODO:
// SDL_BuildAudioCVT
// SDL_ConvertAudio
// SDL_MixAudio
// SDL_MixAudioFormat
// SDL_OpenAudioDevice

// Not implementing:
// SDL_CloseAudio (deprecated)
// SDL_OpenAudio (deprecated)
// SDL_FreeWAV
// SDL_LoadWAV (easier to use node directly?)
// SDL_LoadWAV_RW (easier to use node directly?)
// SDL_GetAudioStatus (deprecated)
// SDL_LockAudio (deprecated)

// REF:
// https://gist.github.com/dmh2000/9519489#file-simple-node-js-async-c-addon-example-L2

namespace sdl2_bindings {

using namespace v8;

METHOD(AudioInit) {
    BEGIN();
    STRINGARG(driverName, 0);
    if (SDL_AudioInit(*driverName) < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(AudioQuit) {
    SDL_AudioQuit();
}

METHOD(ClearQueuedAudio) {
    BEGIN();
    INTARG(dev, 0);
    SDL_ClearQueuedAudio((SDL_AudioDeviceID)dev);
}

METHOD(CloseAudioDevice) {
    BEGIN();
    INTARG(dev, 0);
    SDL_CloseAudioDevice((SDL_AudioDeviceID)dev);
}

METHOD(DequeueAudio) {
    BEGIN();
    INTARG(dev, 0);
    Local<ArrayBufferView> buffer = Local<ArrayBufferView>::Cast(args[1]);
    // FIXME: I think I may need to offset src by buffer->ByteOffset() ???
    void *src = buffer->Buffer()->GetContents().Data();
    RETURN(MK_NUMBER(SDL_DequeueAudio(dev, src, buffer->ByteLength())));
}

METHOD(GetAudioDeviceName) {
    BEGIN();
    INTARG(ix, 0);
    BOOLARG(isCapture, 1);
    auto deviceName = SDL_GetAudioDeviceName(ix, isCapture ? 1 : 0);
    if (deviceName == nullptr) {
        RETURN(MK_NULL());
    } else {
        RETURN(MK_STRING(deviceName));
    }
}

METHOD(GetAudioDeviceStatus) {
    BEGIN();
    INTARG(dev, 0);
    RETURN(MK_NUMBER(SDL_GetAudioDeviceStatus((SDL_AudioDeviceID)dev)));
}

METHOD(GetAudioDriver) {
    BEGIN();
    INTARG(ix, 0);
    auto driverName = SDL_GetAudioDriver(ix);
    if (driverName == nullptr) {
        RETURN(MK_NULL());
    } else {
        RETURN(MK_STRING(driverName));
    }
}

METHOD(GetCurrentAudioDriver) {
    BEGIN();
    auto driverName = SDL_GetCurrentAudioDriver();
    if (driverName == nullptr) {
        RETURN(MK_NULL());
    } else {
        RETURN(MK_STRING(driverName));
    }
}

METHOD(GetNumAudioDevices) {
    BEGIN();
    BOOLARG(isCapture, 0);
    RETURN(MK_NUMBER(SDL_GetNumAudioDevices(isCapture ? 1 : 0)));
}

METHOD(GetNumAudioDrivers) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetNumAudioDrivers()));
}

METHOD(GetQueuedAudioSize) {
    BEGIN();
    INTARG(dev, 0);
    RETURN(MK_NUMBER(SDL_GetQueuedAudioSize(dev)));
}

METHOD(LockAudioDevice) {
    BEGIN();
    INTARG(dev, 0);
    SDL_LockAudioDevice(dev);
}

struct audio_context {
    uv_async_t async_handle;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Uint8 *last_buffer, *buffer;
    int last_buffer_len, buffer_len;
    Persistent<Function> callback;
    Persistent<ArrayBuffer> arrayBuffer;
    Isolate *isolate;
    SDL_AudioFormat format;
};

static void audioCallbackHandler(uv_async_t *handle) {
    audio_context *ctx = (audio_context*)handle;
    Isolate *isolate = ctx->isolate;
    HandleScope scope(isolate);
    if (ctx->buffer != ctx->last_buffer || ctx->buffer_len != ctx->last_buffer_len) {
        ctx->arrayBuffer.Reset(isolate, ArrayBuffer::New(isolate, ctx->buffer, ctx->buffer_len));
        ctx->last_buffer = ctx->buffer;
        ctx->last_buffer_len = ctx->buffer_len;
    }
    Local<Function> fn = ctx->callback.Get(isolate);

    // TODO: could stash these persistently too
    Local<Value> args[1];
    switch (ctx->format) {
        case AUDIO_S8:
            args[0] = Int8Array::New(ctx->arrayBuffer.Get(isolate), 0, ctx->buffer_len);
            break;
        case AUDIO_U8:
            args[0] = Uint8Array::New(ctx->arrayBuffer.Get(isolate), 0, ctx->buffer_len);
            break;
        case AUDIO_S16:
        case AUDIO_S16MSB:
            args[0] = Int16Array::New(ctx->arrayBuffer.Get(isolate), 0, ctx->buffer_len / 2);
            break;
        case AUDIO_U16:
        case AUDIO_U16MSB:
            args[0] = Uint16Array::New(ctx->arrayBuffer.Get(isolate), 0, ctx->buffer_len / 2);
            break;
        case AUDIO_S32:
            args[0] = Int32Array::New(ctx->arrayBuffer.Get(isolate), 0, ctx->buffer_len / 4);
            break;
        case AUDIO_F32:
            args[0] = Float32Array::New(ctx->arrayBuffer.Get(isolate), 0, ctx->buffer_len / 4);
            break;
    }

    fn->Call(isolate->GetCurrentContext(), MK_NULL(), 1, args);

    pthread_mutex_lock(&ctx->mutex);
    pthread_cond_signal(&ctx->cond);
    pthread_mutex_unlock(&ctx->mutex);
}

static void audioCallback(void *userdata, Uint8 *stream, int len) {
    audio_context *ctx = (audio_context*)userdata;
    ctx->buffer = stream;
    ctx->buffer_len = len;
    uv_async_init(uv_default_loop(), &ctx->async_handle, audioCallbackHandler);
    pthread_mutex_lock(&ctx->mutex);
    uv_async_send(&ctx->async_handle);
    pthread_cond_wait(&ctx->cond, &ctx->mutex);    
    pthread_mutex_unlock(&ctx->mutex);
}

METHOD(OpenAudioDevice) {
    BEGIN();

    const char *name = SDL_GetAudioDeviceName(0, 0);

    if (!args[0]->IsFunction()) {
        THROW(Error, "first argument must be a function");
    }

    FNARG(cb, 0);
    
    SDL_AudioSpec desired, obtained;
    desired.freq = 44100;
    desired.format = AUDIO_F32;
    desired.channels = 2;
    desired.samples = 4096;
    desired.callback = audioCallback;

    audio_context *ctx = new audio_context;
    pthread_mutex_init(&ctx->mutex, nullptr);
    pthread_cond_init(&ctx->cond, nullptr);
    ctx->last_buffer = ctx->buffer = nullptr;
    ctx->last_buffer_len = ctx->buffer_len = 0;
    ctx->isolate = isolate;
    desired.userdata = ctx;

    auto deviceId = SDL_OpenAudioDevice(name, 0, &desired, &obtained, 0);

    switch (obtained.format) {
        case AUDIO_S8:
        case AUDIO_U8:
        case AUDIO_S16:
        case AUDIO_S16MSB:
        case AUDIO_U16:
        case AUDIO_U16MSB:
        case AUDIO_S32:
        case AUDIO_F32:
            break;
        default:
            THROW(Error, "unsupported audio format");
            break;
    }

    ctx->callback.Reset(isolate, cb);
    ctx->format = obtained.format;

    RETURN(MK_NUMBER(deviceId));
}

METHOD(PauseAudioDevice) {
    BEGIN();
    INTARG(dev, 0);
    BOOLARG(pauseOn, 1);
    SDL_PauseAudioDevice(dev, pauseOn ? 1 : 0);
}

METHOD(QueueAudio) {
    BEGIN();
    INTARG(dev, 0);
    Local<ArrayBufferView> buffer = Local<ArrayBufferView>::Cast(args[1]);
    // FIXME: I think I may need to offset src by buffer->ByteOffset() ???
    void *src = buffer->Buffer()->GetContents().Data();
    auto res = SDL_QueueAudio(dev, src, buffer->ByteLength());
    if (res < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(UnlockAudioDevice) {
    BEGIN();
    INTARG(dev, 0);
    SDL_UnlockAudioDevice(dev);
}

void InitAudio(Local<Object> exports) {
    NODE_SET_METHOD(exports, "audioInit", AudioInit);
    NODE_SET_METHOD(exports, "audioQuit", AudioQuit);
    NODE_SET_METHOD(exports, "closeAudioDevice", CloseAudioDevice);
    NODE_SET_METHOD(exports, "dequeueAudio", DequeueAudio);
    NODE_SET_METHOD(exports, "getAudioDeviceName", GetAudioDeviceName);
    NODE_SET_METHOD(exports, "getAudioDeviceStatus", GetAudioDeviceStatus);
    NODE_SET_METHOD(exports, "getAudioDriver", GetAudioDriver);
    NODE_SET_METHOD(exports, "getCurrentAudioDriver", GetCurrentAudioDriver);
    NODE_SET_METHOD(exports, "getNumAudioDevices", GetNumAudioDevices);
    NODE_SET_METHOD(exports, "getNumAudioDrivers", GetNumAudioDrivers);
    NODE_SET_METHOD(exports, "getQueuedAudioSize", GetQueuedAudioSize);
    NODE_SET_METHOD(exports, "lockAudioDevice", LockAudioDevice);
    NODE_SET_METHOD(exports, "openAudioDevice", OpenAudioDevice);
    NODE_SET_METHOD(exports, "pauseAudioDevice", PauseAudioDevice);
    NODE_SET_METHOD(exports, "queueAudio", QueueAudio);
    NODE_SET_METHOD(exports, "unlockAudioDevice", UnlockAudioDevice);
}

}

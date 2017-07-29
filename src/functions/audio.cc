#include "deps.h"

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
    UNWRAP(dev, AudioDevice, args[0]);
    SDL_ClearQueuedAudio(dev->deviceId_);
}

METHOD(DequeueAudio) {
    BEGIN();
    UNWRAP(dev, AudioDevice, args[0]);
    Local<ArrayBufferView> buffer = Local<ArrayBufferView>::Cast(args[1]);
    // FIXME: I think I may need to offset src by buffer->ByteOffset() ???
    void *src = buffer->Buffer()->GetContents().Data();
    RETURN(MK_NUMBER(SDL_DequeueAudio(dev->deviceId_, src, buffer->ByteLength())));
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
    UNWRAP(dev, AudioDevice, args[0]);
    RETURN(MK_NUMBER(SDL_GetAudioDeviceStatus(dev->deviceId_)));
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
    UNWRAP(dev, AudioDevice, args[0]);
    RETURN(MK_NUMBER(SDL_GetQueuedAudioSize(dev->deviceId_)));
}

METHOD(LockAudioDevice) {
    BEGIN();
    UNWRAP(dev, AudioDevice, args[0]);
    SDL_LockAudioDevice(dev->deviceId_);
}

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

METHOD(CloseAudioDevice) {
    BEGIN();
    UNWRAP(audioDevice, AudioDevice, args[0]);
    audioDevice->destroy();
}

METHOD(OpenAudioDevice) {
    BEGIN();

    STRINGARG(deviceName, 0);
    BOOLARG(isCapture, 1);
    OBJECTARG(desired, 2);
    INTARG(allowedChanges, 4);

    SDL_AudioSpec sdlDesired, sdlObtained;

    GET_CONTEXT();

    sdlDesired.freq = desired->Get(ctx, SYM(freq)).ToLocalChecked()->Int32Value();
    sdlDesired.format = desired->Get(ctx, SYM(format)).ToLocalChecked()->Int32Value();
    sdlDesired.channels = desired->Get(ctx, SYM(channels)).ToLocalChecked()->Int32Value();
    sdlDesired.samples = desired->Get(ctx, SYM(samples)).ToLocalChecked()->Int32Value();

    audio_context *audioContext = nullptr;
    auto callback = desired->Get(ctx, SYM(callback)).ToLocalChecked();
    if (callback->IsFunction()) {
        audioContext = new audio_context;
        pthread_mutex_init(&audioContext->mutex, nullptr);
        pthread_cond_init(&audioContext->cond, nullptr);
        audioContext->last_buffer = audioContext->buffer = nullptr;
        audioContext->last_buffer_len = audioContext->buffer_len = 0;
        audioContext->isolate = isolate;
        audioContext->callback.Reset(isolate, Local<Function>::Cast(callback));
        sdlDesired.callback = audioCallback;
        sdlDesired.userdata = audioContext;
    } else {
        sdlDesired.callback = nullptr;
        sdlDesired.userdata = nullptr;
    }

    auto deviceId = SDL_OpenAudioDevice(*deviceName, isCapture ? 1 : 0, &sdlDesired, &sdlObtained, allowedChanges);
    if (deviceId == 0) {
        THROW_SDL_ERROR();
    }

    switch (sdlObtained.format) {
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
            delete audioContext;
            audioContext = nullptr;
            SDL_CloseAudioDevice(deviceId);
            THROW(Error, "unsupported audio format");
            break;
    }

    if (audioContext != nullptr) {
        audioContext->format = sdlObtained.format;
    }

    if (!args[3]->IsNull() && !args[3]->IsUndefined()) {
        OBJECTARG(obtained, 3);
        SET_KEY(obtained, SYM(freq), MK_NUMBER(sdlObtained.freq));
        SET_KEY(obtained, SYM(format), MK_NUMBER(sdlObtained.format));
        SET_KEY(obtained, SYM(channels), MK_NUMBER(sdlObtained.channels));
        SET_KEY(obtained, SYM(silence), MK_NUMBER(sdlObtained.silence));
        SET_KEY(obtained, SYM(samples), MK_NUMBER(sdlObtained.samples));
        SET_KEY(obtained, SYM(size), MK_NUMBER(sdlObtained.size));
    }

    RETURN(AudioDevice::NewInstance(isolate, deviceId, audioContext));
}

METHOD(PauseAudioDevice) {
    BEGIN();
    UNWRAP(dev, AudioDevice, args[0]);
    BOOLARG(pauseOn, 1);
    SDL_PauseAudioDevice(dev->deviceId_, pauseOn ? 1 : 0);
}

METHOD(QueueAudio) {
    BEGIN();
    UNWRAP(dev, AudioDevice, args[0]);
    Local<ArrayBufferView> buffer = Local<ArrayBufferView>::Cast(args[1]);
    // FIXME: I think I may need to offset src by buffer->ByteOffset() ???
    void *src = buffer->Buffer()->GetContents().Data();
    auto res = SDL_QueueAudio(dev->deviceId_, src, buffer->ByteLength());
    if (res < 0) {
        THROW_SDL_ERROR();
    }
}

METHOD(UnlockAudioDevice) {
    BEGIN();
    UNWRAP(dev, AudioDevice, args[0]);
    SDL_UnlockAudioDevice(dev->deviceId_);
}

void InitAudio(Local<Object> exports) {
    NODE_SET_METHOD(exports, "audioInit", AudioInit);
    NODE_SET_METHOD(exports, "audioQuit", AudioQuit);
    NODE_SET_METHOD(exports, "dequeueAudio", DequeueAudio);
    NODE_SET_METHOD(exports, "getAudioDeviceName", GetAudioDeviceName);
    NODE_SET_METHOD(exports, "getAudioDeviceStatus", GetAudioDeviceStatus);
    NODE_SET_METHOD(exports, "getAudioDriver", GetAudioDriver);
    NODE_SET_METHOD(exports, "getCurrentAudioDriver", GetCurrentAudioDriver);
    NODE_SET_METHOD(exports, "getNumAudioDevices", GetNumAudioDevices);
    NODE_SET_METHOD(exports, "getNumAudioDrivers", GetNumAudioDrivers);
    NODE_SET_METHOD(exports, "getQueuedAudioSize", GetQueuedAudioSize);
    NODE_SET_METHOD(exports, "lockAudioDevice", LockAudioDevice);
    NODE_SET_METHOD(exports, "closeAudioDevice", CloseAudioDevice);
    NODE_SET_METHOD(exports, "openAudioDevice", OpenAudioDevice);
    NODE_SET_METHOD(exports, "pauseAudioDevice", PauseAudioDevice);
    NODE_SET_METHOD(exports, "queueAudio", QueueAudio);
    NODE_SET_METHOD(exports, "unlockAudioDevice", UnlockAudioDevice);
}

}

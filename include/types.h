#ifndef NODESDL2_TYPES_H
#define NODESDL2_TYPES_H

namespace sdl2_bindings {

using namespace v8;

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

    ~audio_context() {
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mutex);
        callback.Reset();
        arrayBuffer.Reset();
    }
};

class AudioDevice : public node::ObjectWrap {
public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_AudioDeviceID deviceId, audio_context *ctx);

    static METHOD(New);

    void destroy();
    bool isDestroyed();

    SDL_AudioDeviceID deviceId_;

private:
    static v8::Persistent<v8::Function> constructor;
    AudioDevice(SDL_AudioDeviceID deviceId, audio_context *ctx);
    ~AudioDevice();

    audio_context *audioContext_;
    bool destroyed_;
};

class Cursor : public node::ObjectWrap {
public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_Cursor *cursor, bool userCreated);

    static METHOD(New);

    bool isUserCreated();
    void destroy();
    bool isDestroyed();

    SDL_Cursor *cursor_;

private:
    static v8::Persistent<v8::Function> constructor;
    Cursor(SDL_Cursor *cursor, bool userCreated);
    ~Cursor();

    bool userCreated_;
    bool destroyed_;
};

class GameController : public node::ObjectWrap {

public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_GameController *gameController);

    SDL_GameController *gameController_;

    void close();
    bool isClosed();
    
private:
    static v8::Persistent<v8::Function> constructor;
    GameController(SDL_GameController *gameController);
    ~GameController();

    bool closed_;

    static METHOD(New);
};

class GLContext : public node::ObjectWrap {

public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_GLContext ctx);

    SDL_GLContext ctx_;

    void destroy();
    bool isDestroyed();
    
private:
    static v8::Persistent<v8::Function> constructor;
    GLContext(SDL_GLContext ctx);
    ~GLContext();

    bool destroyed_;

    static METHOD(New);
};

class Joystick : public node::ObjectWrap {

public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_Joystick *joystick);

    SDL_Joystick *joystick_;

    void close();
    bool isClosed();
    
private:
    static v8::Persistent<v8::Function> constructor;
    Joystick(SDL_Joystick *joystick);
    ~Joystick();

    bool closed_;

    static METHOD(New);
};

class Renderer : public node::ObjectWrap {
public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_Renderer *renderer);

	SDL_Renderer *renderer_;

	static METHOD(New);

    void destroy();

private:
    static v8::Persistent<v8::Function> constructor;
    Renderer(SDL_Renderer *renderer);
    ~Renderer();
};

class Surface : public node::ObjectWrap {
public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_Surface *surface, bool owned);

    static METHOD(New);
    static GETTER(GetWidth);
    static GETTER(GetHeight);
    static GETTER(GetPitch);
    static GETTER(GetStride);
    static GETTER(GetData);
    static GETTER(GetFormat);

    SDL_Surface *surface_;

private:
    static v8::Persistent<v8::Function> constructor;
    Surface(SDL_Surface *surface, bool owned);
    ~Surface();

    bool owned_;
};

class Texture : public node::ObjectWrap {
public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_Texture *texture);

	SDL_Texture *texture_;

	static METHOD(New);

    void destroy();

private:
    static v8::Persistent<v8::Function> constructor;
    Texture(SDL_Texture *texture);
    ~Texture();
};

class Window : public node::ObjectWrap {
public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_Window *window);

	SDL_Window *window_;

	static METHOD(New);

    void destroy();

private:
    static v8::Persistent<v8::Function> constructor;
    Window(SDL_Window *window);
    ~Window();
};

}

#endif
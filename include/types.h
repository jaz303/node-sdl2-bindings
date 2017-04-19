#ifndef NODESDL2_TYPES_H
#define NODESDL2_TYPES_H

namespace sdl2_bindings {

using namespace v8;

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
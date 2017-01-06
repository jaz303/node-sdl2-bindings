#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cairo/cairo.h>

namespace nsdl {

using namespace v8;

class Window : public node::ObjectWrap {
public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_Window *window);

    static GETTER(Id);
    static GETTER(GetSurface);
    static GETTER(GetDisplayIndex);
    static GETTER(GetFlags);

    static METHOD(New);
    static METHOD(Update);
    static METHOD(UpdateRects);
    static METHOD(CreateRenderer);
    static METHOD(Hide);
    static METHOD(Show);
    static METHOD(Maximize);
    static METHOD(Minimize);
    static METHOD(Raise);
    static METHOD(Restore);
    static METHOD(ShowMessageBox);
    static METHOD(ShowSimpleMessageBox);
    static METHOD(GetGrab);
    static METHOD(SetGrab);
    static METHOD(SetIcon);
    static METHOD(GetMaximumSize);
    static METHOD(GetMinimumSize);
    static METHOD(SetMaximumSize);
    static METHOD(SetMinimumSize);
    static METHOD(GetDisplayMode);
    static METHOD(GetPixelFormat);
    static METHOD(SetBordered);
    static METHOD(SetDisplayMode);
    static METHOD(SetFullscreen);
    static METHOD(GetSize);
    static METHOD(SetSize);
    static METHOD(GetPosition);
    static METHOD(SetPosition);
    static METHOD(GetTitle);
    static METHOD(SetTitle);
    static METHOD(GetBrightness);
    static METHOD(SetBrightness);

#ifdef SDL_GE_2_0_5
    static METHOD(GetBordersSize);
    static METHOD(SetResizable);
    static METHOD(GetOpacity);
    static METHOD(SetOpacity);
#endif

    SDL_Window *window_;

    void destroy();

private:
    static v8::Persistent<v8::Function> constructor;
    Window(SDL_Window *window);
    ~Window();
};

class Surface : public node::ObjectWrap {
    friend class Renderer;
    friend class Window;

public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_Surface *surface, bool owned);

    static METHOD(New);
    static METHOD(Blit);
    static METHOD(BlitRect);
    static METHOD(BlitRectScaled);
    static METHOD(BlitScaled);
    static METHOD(Clear);
    static METHOD(FillRect);
    static METHOD(MapRGB);
    static METHOD(MapRGBA);

    static GETTER(GetWidth);
    static GETTER(GetHeight);
    static GETTER(GetPitch);

    static METHOD(GetContext);
    
private:
    static v8::Persistent<v8::Function> constructor;
    Surface(SDL_Surface *surface, bool owned);
    ~Surface();

    SDL_Surface *surface_;
    bool owned_;
    cairo_t *cairoContext_;
};

class Renderer : public node::ObjectWrap {
public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_Renderer *renderer);

    static METHOD(New);

    static GETTER(GetDrawColor);
    static SETTER(SetDrawColor);
    static GETTER(GetBlendMode);
    static SETTER(SetBlendMode);

    static METHOD(CreateTexture);
    static METHOD(CreateTextureFromSurface);

    static METHOD(Clear);
    
    static METHOD(Copy);
    static METHOD(CopyEx);
    static METHOD(CopyRect);
    static METHOD(CopyRectEx);

    static METHOD(DrawLine);
    // TODO: static METHOD(DrawLines);
    static METHOD(DrawPoint);
    // TODO: static METHOD(DrawPoints);
    static METHOD(DrawRect);
    // TODO: static METHOD(DrawRects);
    static METHOD(FillRect);
    // TODO: static METHOD(FillRects);
    static METHOD(Present);

    static METHOD(SetRenderTarget);
    static METHOD(SetDefaultRenderTarget);

private:
    static v8::Persistent<v8::Function> constructor;
    Renderer(SDL_Renderer *renderer);
    ~Renderer();

    SDL_Renderer *renderer_;
};

class Texture : public node::ObjectWrap {
    friend class Renderer;

public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_Texture *texture);

private:
    static v8::Persistent<v8::Function> constructor;
    Texture(SDL_Texture *texture);
    ~Texture();

    static METHOD(New);

    static METHOD(Bind);
    static METHOD(Unbind);

    static GETTER(GetWidth);
    static GETTER(GetHeight);
    static GETTER(GetFormat);
    static GETTER(GetAccess);

    SDL_Texture *texture_;
};

class Context2D : public node::ObjectWrap {

public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, cairo_t *ctx);

private:
    static v8::Persistent<v8::Function> constructor;
    Context2D(cairo_t *cr);
    ~Context2D();

    static METHOD(New);

    static METHOD(Save);
    static METHOD(Restore);

    cairo_t *ctx_;
};

class GLContext : public node::ObjectWrap {

public:
    static void Init(Isolate *isolate);
    static Local<Object> NewInstance(Isolate *isolate, SDL_GLContext ctx);

    SDL_GLContext ctx_;

private:
    static v8::Persistent<v8::Function> constructor;
    GLContext(SDL_GLContext ctx);
    ~GLContext();

    static METHOD(New);
    static METHOD(Destroy);

    void destroy();
    bool destroyed_;
};

}
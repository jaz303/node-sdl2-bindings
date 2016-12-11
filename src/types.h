#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace nsdl {

using namespace v8;

class Window : public node::ObjectWrap {
public:
	static void Init(Isolate *isolate);
	static Local<Object> NewInstance(Isolate *isolate, SDL_Window *window);

	static METHOD(New);
	static METHOD(Update);

	static GETTER(GetSurface);
	static GETTER(GetTitle);
	static SETTER(SetTitle);

	SDL_Window *window_;

	void destroy();

private:
	static v8::Persistent<v8::Function> constructor;
	Window(SDL_Window *window);
	~Window();
};

class Surface : public node::ObjectWrap {
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

	SDL_Surface *surface_;
	bool owned_;

private:
	static v8::Persistent<v8::Function> constructor;
	Surface(SDL_Surface *surface, bool owned);
	~Surface();
};

}
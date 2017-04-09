#include <iostream>
#include <node.h>
#include <v8.h>

namespace sdl2_bindings {

using namespace v8;

#define DEFSYM(name) v8::Persistent<v8::String> sym_##name;
#include "symbols.x"
#undef DEFSYM

void InitSymbols(Isolate *isolate) {
	#define DEFSYM(name) sym_##name.Reset(isolate, String::NewFromUtf8(isolate, #name));
	#include "symbols.x"
	#undef DEFSYM
}

}
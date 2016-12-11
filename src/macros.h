#define NS_BEGIN() \
	namespace nsdl { \
		using namespace v8;

#define NS_END() \
	}

#define UNUSED(exp) \
	do { (void)(exp); } while(0)

#define METHOD(name) \
	void name(const FunctionCallbackInfo<Value>& args)

#define BEGIN() \
	Isolate* isolate = args.GetIsolate(); \
	UNUSED(isolate)

#define THROW(errorClass, message) \
	isolate->ThrowException(Exception::errorClass( \
		String::NewFromUtf8(isolate, message) \
	)); \
	return

#define SYM(name) \
	sym_##name.Get(isolate)

#define INT32(val) \
	(val)->Int32Value()

#define UINT32(val) \
	(val)->Uint32Value()

#define MK_NUMBER(val) v8::Number::New(isolate, (val))
#define MK_STRING(val) v8::String::NewFromUtf8(isolate, (val))
#define MK_BOOL(val) v8::Boolean::New(isolate, (val))
#define MK_TRUE() v8::True(isolate)
#define MK_FALSE() v8::False(isolate)

#define UNWRAP(name, type, from) \
	type *name = node::ObjectWrap::Unwrap<type>((from)->ToObject())

#define NARGS(n) \
	if (args.Length() != n) { \
		THROW(TypeError, "argument error"); \
	}

#define NARGS2(min, max) \
	if (args.Length() < min || args.Length() > max) { \
		THROW(TypeError, "argument error"); \
	}

#define ARG(vartype, name, ix, typecheck, cast) \
	if (!args[ix]->typecheck()) { \
		THROW(TypeError, "argument error"); \
	} \
	vartype name = args[ix]->cast()

#define OBJECTARG(name, ix) \
	if (!args[ix]->IsObject()) { \
		THROW(TypeError, "argument error"); \
	} \
	Local<Object> name = args[ix]->ToObject()

#define STRINGARG(name, ix) \
	if (!args[ix]->IsString()) { \
		THROW(TypeError, "argument error"); \
	} \
	String::Utf8Value name(args[ix])

#define INTARG(name, ix) \
	ARG(int, name, ix, IsNumber, Int32Value)

#define UINT32ARG(name, ix) \
	ARG(uint32_t, name, ix, IsNumber, Uint32Value)

#define RETURN(val) args.GetReturnValue().Set(val)
#define RETURN_BOOL(val) RETURN(MK_BOOL(val))
#define RETURN_TRUE() RETURN(MK_TRUE())
#define RETURN_FALSE() RETURN(MK_FALSE())

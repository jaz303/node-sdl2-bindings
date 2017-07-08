#define NS_BEGIN() \
    namespace nsdl { \
        using namespace v8;

#define NS_END() \
    }

#define DEFINE_METHOD(tpl, name, method) \
    NODE_SET_PROTOTYPE_METHOD(tpl, name, method)

#define DEFINE_GETTER(tpl, name, method) \
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, name), method)

#define DEFINE_ACCESSOR(tpl, name, getter, setter) \
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, name), getter, setter)

#define UNUSED(exp) \
    do { (void)(exp); } while(0)

#define METHOD(name) \
    void name(const FunctionCallbackInfo<Value>& args)

#define GETTER(name) \
    void name(Local<String> property, const PropertyCallbackInfo<Value>& args)

#define SETTER(name) \
    void name(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& args)

#define BEGIN() \
    Isolate* isolate = args.GetIsolate(); \
    UNUSED(isolate)

#define THROW(errorClass, message) \
    isolate->ThrowException(Exception::errorClass( \
        String::NewFromUtf8(isolate, message) \
    )); \
    return

#define THROW_SDL_ERROR() \
    THROW(Error, SDL_GetError())

#define SYM(name) \
    sym_##name.Get(isolate)

#define INT32(val) \
    (val)->Int32Value()

#define UINT32(val) \
    (val)->Uint32Value()

#define NUMBER(val) \
    (val)->NumberValue()

#define MK_NUMBER(val) v8::Number::New(isolate, (val))
#define MK_STRING(val) v8::String::NewFromUtf8(isolate, (val))
#define MK_BOOL(val) v8::Boolean::New(isolate, (val))
#define MK_TRUE() v8::True(isolate)
#define MK_FALSE() v8::False(isolate)
#define MK_NULL() v8::Null(isolate)
#define MK_OBJECT() v8::Object::New(isolate)

#define UNWRAP(name, type, from) \
    type *name = node::ObjectWrap::Unwrap<type>((from)->ToObject())

#define UNWRAP_ME(name, type) \
    type *name = node::ObjectWrap::Unwrap<type>(args.This())

#define ARG(vartype, name, ix, cast) \
    vartype name = args[ix]->cast()

#define OBJECTARG(name, ix) \
    Local<Object> name = args[ix]->ToObject()

#define ARRAYARG(name, ix) \
    Local<Array> name = Local<Array>::Cast(args[ix])

#define FNARG(name, ix) \
    Local<Function> name = Local<Function>::Cast(args[ix])

#define CASTARG(name, type, ix) \
    Local<type> name = Local<type>::Cast(args[ix])

#define STRINGARG(name, ix) \
    String::Utf8Value name(args[ix])

#define DOUBLEARG(name, ix) \
    ARG(double, name, ix, NumberValue)

#define BOOLARG(name, ix) \
    ARG(bool, name, ix, BooleanValue)

#define INTARG(name, ix) \
    ARG(int, name, ix, Int32Value)

#define UINT32ARG(name, ix) \
    ARG(uint32_t, name, ix, Uint32Value)

#define RETURN(val) args.GetReturnValue().Set(val)
#define RETURN_INT(val) RETURN(MK_NUMBER(val))
#define RETURN_BOOL(val) RETURN(MK_BOOL(val))
#define RETURN_TRUE() RETURN(MK_TRUE())
#define RETURN_FALSE() RETURN(MK_FALSE())

#define GET_CONTEXT() auto ctx = isolate->GetCurrentContext()
#define SET_KEY(obj, key, val) ((void)obj->CreateDataProperty(ctx, key, val))
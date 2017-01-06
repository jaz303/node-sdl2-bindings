v8::Persistent<v8::Function> GLContext::constructor;

GLContext::GLContext(SDL_GLContext ctx) : ctx_(ctx), destroyed_(false) {}
GLContext::~GLContext() { destroy(); }

void GLContext::destroy() {
    if (!destroyed_) {
        SDL_GL_DeleteContext(ctx_);
        destroyed_ = true;
    }
}

void GLContext::Init(Isolate *isolate) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "GLContext"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    DEFINE_METHOD(tpl, "destroy", Destroy);
    
    constructor.Reset(isolate, tpl->GetFunction());
}

Local<Object> GLContext::NewInstance(Isolate *isolate, SDL_GLContext ctx) {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
    GLContext *c = new GLContext(ctx);
    c->Wrap(instance);
    return instance;
}

METHOD(GLContext::New) {}

METHOD(GLContext::Destroy) {
    UNWRAP_ME(self, GLContext);
    self->destroy();
}

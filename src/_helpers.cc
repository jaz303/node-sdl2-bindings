void extractRect(Isolate *isolate, Local<Object> obj, SDL_Rect *rect) {
    GET_CONTEXT();
    rect->x = obj->Get(ctx, SYM(x)).ToLocalChecked()->Int32Value();
    rect->y = obj->Get(ctx, SYM(y)).ToLocalChecked()->Int32Value();
    rect->w = obj->Get(ctx, SYM(width)).ToLocalChecked()->Int32Value();
    rect->h = obj->Get(ctx, SYM(height)).ToLocalChecked()->Int32Value();
}

void extractDisplayMode(Isolate *isolate, Local<Object> obj, SDL_DisplayMode *dm) {
    GET_CONTEXT();
    dm->format = obj->Get(ctx, SYM(format)).ToLocalChecked()->Uint32Value();
    dm->w = obj->Get(ctx, SYM(width)).ToLocalChecked()->Int32Value();
    dm->h = obj->Get(ctx, SYM(height)).ToLocalChecked()->Int32Value();
    dm->refresh_rate = obj->Get(ctx, SYM(refreshRate)).ToLocalChecked()->Int32Value();
    dm->driverdata = nullptr;
}

int decodeMessageBoxFlags(Local<Value> val) {
    if (val->IsString()) {
        auto str = *String::Utf8Value(val->ToString());
        if (strcmp(str, "error") == 0) {
            return SDL_MESSAGEBOX_ERROR;
        } else if (strcmp(str, "warning") == 0) {
            return SDL_MESSAGEBOX_WARNING;
        } else {
            return SDL_MESSAGEBOX_INFORMATION;
        }
    } else {
        return UINT32(val);    
    }
}

// TODO: SDL_ShowMessageBox
void showMessageBox(const FunctionCallbackInfo<Value>& args, SDL_Window *window) {
    BEGIN();
    THROW(Error, "SDL_ShowMessageBox is not implemented");
}

void showSimpleMessageBox(const FunctionCallbackInfo<Value>& args, SDL_Window *window) {
    BEGIN();
    if (args.Length() < 2) {
        THROW(Error, "message box requires at least 2 arguments");
    }
    STRINGARG(title, 0);
    STRINGARG(message, 1);
    uint32_t flags = SDL_MESSAGEBOX_INFORMATION;
    if (args.Length() > 2) {
        flags = decodeMessageBoxFlags(args[2]);
    }
    if (SDL_ShowSimpleMessageBox(flags, *title, *message, window) != 0) {
        THROW_SDL_ERROR();
    }
}
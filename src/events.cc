#define EVKEY(name) \
    Local<Object> evinfo = Object::New(isolate); \
    evt->CreateDataProperty(ctx, SYM(name), evinfo)

#define EVSET(key, value) \
    evinfo->CreateDataProperty(ctx, SYM(key), value)

void populateEvent(Isolate *isolate, Local<Object> evt, SDL_Event *sdlEvent) {
    auto ctx = isolate->GetCurrentContext();
    evt->CreateDataProperty(ctx, SYM(type), Number::New(isolate, sdlEvent->type));
    evt->CreateDataProperty(ctx, SYM(timestamp), Number::New(isolate, sdlEvent->window.timestamp));
    switch (sdlEvent->type) {
        case SDL_WINDOWEVENT:
        {
            EVKEY(window);
            EVSET(windowId, MK_NUMBER(sdlEvent->window.windowID));
            EVSET(event, MK_NUMBER(sdlEvent->window.event));
            EVSET(data1, MK_NUMBER(sdlEvent->window.data1));
            EVSET(data2, MK_NUMBER(sdlEvent->window.data2));
            break;
        }
        case SDL_SYSWMEVENT:
        {
            // TODO: is there any useful way to support this?
            break;
        }
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            EVKEY(key);
            EVSET(windowId, MK_NUMBER(sdlEvent->key.windowID));
            EVSET(state, MK_NUMBER(sdlEvent->key.state));
            EVSET(repeat, MK_NUMBER(sdlEvent->key.repeat));
            EVSET(scancode, MK_NUMBER(sdlEvent->key.keysym.scancode));
            EVSET(sym, MK_NUMBER(sdlEvent->key.keysym.sym));
            EVSET(mod, MK_NUMBER(sdlEvent->key.keysym.mod));
            break;
        }
        case SDL_MOUSEMOTION:
        {
            EVKEY(motion);
            EVSET(windowId, MK_NUMBER(sdlEvent->motion.windowID));
            EVSET(which, MK_NUMBER(sdlEvent->motion.which));
            EVSET(state, MK_NUMBER(sdlEvent->motion.state));
            EVSET(x, MK_NUMBER(sdlEvent->motion.x));
            EVSET(y, MK_NUMBER(sdlEvent->motion.y));
            EVSET(xRel, MK_NUMBER(sdlEvent->motion.xrel));
            EVSET(yRel, MK_NUMBER(sdlEvent->motion.yrel));
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            EVKEY(button);
            EVSET(windowId, MK_NUMBER(sdlEvent->button.windowID));
            EVSET(which, MK_NUMBER(sdlEvent->button.which));
            EVSET(button, MK_NUMBER(sdlEvent->button.button));
            EVSET(state, MK_NUMBER(sdlEvent->button.state));
            EVSET(clicks, MK_NUMBER(sdlEvent->button.clicks));
            EVSET(x, MK_NUMBER(sdlEvent->button.x));
            EVSET(y, MK_NUMBER(sdlEvent->button.y));
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            EVKEY(wheel);
            EVSET(windowId, MK_NUMBER(sdlEvent->wheel.windowID));
            EVSET(which, MK_NUMBER(sdlEvent->wheel.which));
            EVSET(x, MK_NUMBER(sdlEvent->wheel.x));
            EVSET(y, MK_NUMBER(sdlEvent->wheel.y));
            EVSET(direction, MK_NUMBER(sdlEvent->wheel.direction));
            break;
        }
        case SDL_JOYAXISMOTION:
        {
            EVKEY(jaxis);
            EVSET(which, MK_NUMBER(sdlEvent->jaxis.which));
            EVSET(axis, MK_NUMBER(sdlEvent->jaxis.axis));
            EVSET(value, MK_NUMBER(sdlEvent->jaxis.value));
            break;
        }
        case SDL_JOYBALLMOTION:
        {
            EVKEY(jball);
            EVSET(which, MK_NUMBER(sdlEvent->jball.which));
            EVSET(ball, MK_NUMBER(sdlEvent->jball.ball));
            EVSET(xRel, MK_NUMBER(sdlEvent->jball.xrel));
            EVSET(yRel, MK_NUMBER(sdlEvent->jball.yrel));
            break;
        }
        case SDL_JOYHATMOTION:
        {
            EVKEY(jhat);
            EVSET(which, MK_NUMBER(sdlEvent->jhat.which));
            EVSET(hat, MK_NUMBER(sdlEvent->jhat.hat));
            EVSET(value, MK_NUMBER(sdlEvent->jhat.value));
            break;
        }
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
        {
            EVKEY(jbutton);
            EVSET(which, MK_NUMBER(sdlEvent->jbutton.which));
            EVSET(button, MK_NUMBER(sdlEvent->jbutton.button));
            EVSET(state, MK_NUMBER(sdlEvent->jbutton.state));
            break;
        }
        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED:
        {
            EVKEY(jdevice);
            EVSET(which, MK_NUMBER(sdlEvent->jdevice.which));
            break;
        }
        case SDL_CONTROLLERAXISMOTION:
        {
            EVKEY(caxis);
            EVSET(which, MK_NUMBER(sdlEvent->caxis.which));
            EVSET(axis, MK_NUMBER(sdlEvent->caxis.axis));
            EVSET(value, MK_NUMBER(sdlEvent->caxis.value));
            break;
        }
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
        {
            EVKEY(cbutton);
            EVSET(which, MK_NUMBER(sdlEvent->cbutton.which));
            EVSET(button, MK_NUMBER(sdlEvent->cbutton.button));
            EVSET(state, MK_NUMBER(sdlEvent->cbutton.state));
            break;
        }
        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED:
        case SDL_CONTROLLERDEVICEREMAPPED:
        {
            EVKEY(cdevice);
            EVSET(which, MK_NUMBER(sdlEvent->cdevice.which));
            break;
        }
        case SDL_FINGERMOTION:
        case SDL_FINGERDOWN:
        case SDL_FINGERUP:
        {
            EVKEY(tfinger);
            EVSET(touchId, MK_NUMBER(sdlEvent->tfinger.touchId));
            EVSET(fingerId, MK_NUMBER(sdlEvent->tfinger.fingerId));
            EVSET(x, MK_NUMBER(sdlEvent->tfinger.x));
            EVSET(y, MK_NUMBER(sdlEvent->tfinger.y));
            EVSET(dx, MK_NUMBER(sdlEvent->tfinger.dx));
            EVSET(dy, MK_NUMBER(sdlEvent->tfinger.dy));
            EVSET(pressure, MK_NUMBER(sdlEvent->tfinger.pressure));
            break;
        }
        // TODO: compile-time version check to check support for other drop ops
        case SDL_DROPFILE:
        {
            EVKEY(drop);
            if (sdlEvent->drop.file) {
                EVSET(file, MK_STRING(sdlEvent->drop.file));
                SDL_free(sdlEvent->drop.file);
            }
            break;
        }
        case SDL_AUDIODEVICEADDED:
        case SDL_AUDIODEVICEREMOVED:
        {
            EVKEY(adevice);
            EVSET(which, MK_NUMBER(sdlEvent->adevice.which));
            EVSET(isCapture, MK_BOOL(sdlEvent->adevice.iscapture != 0));
            break;
        }
        case SDL_TEXTEDITING:
        {
            EVKEY(edit);
            EVSET(windowId, MK_NUMBER(sdlEvent->edit.windowID));
            EVSET(text, MK_STRING(sdlEvent->edit.text));
            EVSET(start, MK_NUMBER(sdlEvent->edit.start));
            EVSET(length, MK_NUMBER(sdlEvent->edit.length));
            break;
        }
        case SDL_TEXTINPUT:
        {
            EVKEY(text);
            EVSET(windowId, MK_NUMBER(sdlEvent->text.windowID));
            EVSET(text, MK_STRING(sdlEvent->text.text));
            break;
        }
        case SDL_USEREVENT:
        {
            EVKEY(user);
            EVSET(windowId, MK_NUMBER(sdlEvent->user.windowID));
            EVSET(code, MK_NUMBER(sdlEvent->user.code));
            // TODO: handle data1 and data2
        }
        // TODO: SDL_DOLLARGESTURE
        // TODO: SDL_DOLLARRECORD
        // TODO: SDL_MULTIGESTURE
    }
}

// SDL_AddEventWatch
// SDL_DelEventWatch

METHOD(EventState) {
    BEGIN();
    NARGS(2);
    UINT32ARG(type, 0);
    INTARG(state, 1);
    auto res = SDL_EventState(type, state);
    RETURN(MK_NUMBER(res));
}

// SDL_FilterEvents

METHOD(FlushEvent) {
    BEGIN();
    NARGS(1);
    UINT32ARG(type, 0);
    SDL_FlushEvent(type);
}

METHOD(FlushEvents) {
    BEGIN();
    NARGS(2);
    UINT32ARG(minType, 0);
    UINT32ARG(maxType, 1);
    SDL_FlushEvents(minType, maxType);
}

// SDL_GetEventFilter

METHOD(GetEventState) {
    BEGIN();
    NARGS(1);
    UINT32ARG(type, 0);
    auto res = SDL_GetEventState(type);
    RETURN(MK_NUMBER(res));
}

METHOD(GetNumTouchDevices) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetNumTouchDevices()));
}

METHOD(GetNumTouchFingers) {
    BEGIN();
    NARGS(1);
    INTARG(touchId, 0);
    RETURN(MK_NUMBER(SDL_GetNumTouchFingers(touchId)));
}

// SDL_GetTouchDevice
// SDL_GetTouchFinger

METHOD(HasEvent) {
    BEGIN();
    NARGS(1);
    UINT32ARG(type, 0);
    RETURN(MK_BOOL(SDL_HasEvent(type) == SDL_TRUE));
}

METHOD(HasEvents) {
    BEGIN();
    NARGS(2);
    UINT32ARG(minType, 0);
    UINT32ARG(maxType, 1);
    RETURN(MK_BOOL(SDL_HasEvents(minType, maxType) == SDL_TRUE));
}

// TODO: should support a file pointer as well as filename
METHOD(LoadDollarTemplates) {
    BEGIN();
    NARGS(2);
    INTARG(touchId, 0);
    STRINGARG(src, 1);
    auto ops = SDL_RWFromFile(*src, "rb");
    if (ops == NULL) {
        THROW_SDL_ERROR();
    }
    auto res = SDL_LoadDollarTemplates(touchId, ops);
    SDL_RWclose(ops);
    if (res <= 0) {
        THROW_SDL_ERROR();
    }
    RETURN(MK_NUMBER(res));
}

// SDL_PeepEvents

METHOD(PollEvent) {
    BEGIN();

    NARGS(1);
    OBJECTARG(evt, 0);

    SDL_Event sdlEvent;
    if (SDL_PollEvent(&sdlEvent)) {
        populateEvent(isolate, evt, &sdlEvent);
        RETURN(MK_TRUE());
    } else {
        RETURN(MK_FALSE());
    }
}

METHOD(PumpEvents) {
    SDL_PumpEvents();
}

// SDL_PushEvent

METHOD(QuitRequested) {
    BEGIN();
    RETURN(MK_BOOL(SDL_QuitRequested()));
}

METHOD(RecordGesture) {
    BEGIN();
    NARGS(1);
    INTARG(touchId, 0);
    auto res = SDL_RecordGesture(touchId);
    if (res == 0) {
        THROW(Error, "invalid touch device index");
    }
}

METHOD(RegisterEvents) {
    BEGIN();
    NARGS(1);
    INTARG(numEvents, 0);
    auto res = SDL_RegisterEvents(numEvents);
    RETURN(MK_NUMBER(res));
}

METHOD(SaveAllDollarTemplates) {
    BEGIN();
    NARGS(1);
    STRINGARG(dst, 0);
    auto ops = SDL_RWFromFile(*dst, "wb");
    if (ops == NULL) {
        THROW_SDL_ERROR();
    }
    auto res = SDL_SaveAllDollarTemplates(ops);
    SDL_RWclose(ops);
    if (res == 0) {
        THROW_SDL_ERROR();
    }
    RETURN(MK_NUMBER(res));
}

METHOD(SaveDollarTemplate) {
    BEGIN();
    NARGS(2);
    INTARG(gestureId, 0);
    STRINGARG(dst, 0);
    auto ops = SDL_RWFromFile(*dst, "wb");
    if (ops == NULL) {
        THROW_SDL_ERROR();
    }
    auto res = SDL_SaveDollarTemplate(gestureId, ops);
    SDL_RWclose(ops);
    if (res == 0) {
        THROW_SDL_ERROR();
    }
}

// SDL_SetEventFilter

METHOD(WaitEvent) {
    BEGIN();

    NARGS2(1, 2);
    OBJECTARG(evt, 0);

    SDL_Event sdlEvent;
    if (args.Length() == 2) {
        INTARG(timeout, 1);
        int res = SDL_WaitEventTimeout(&sdlEvent, timeout);
        if (res) {
            populateEvent(isolate, evt, &sdlEvent);
            RETURN(MK_TRUE());
        } else {
            RETURN(MK_FALSE());
        }
    } else {
        SDL_WaitEvent(&sdlEvent);   
        populateEvent(isolate, evt, &sdlEvent);
    }
}

void initEvents(Local<Object> exports) {
    NODE_SET_METHOD(exports, "eventState", EventState);
    NODE_SET_METHOD(exports, "flushEvent", FlushEvent);
    NODE_SET_METHOD(exports, "flushEvents", FlushEvents);
    NODE_SET_METHOD(exports, "getEventState", GetEventState);
    NODE_SET_METHOD(exports, "getNumTouchDevices", GetNumTouchDevices);
    NODE_SET_METHOD(exports, "getNumTouchFingers", GetNumTouchFingers);
    NODE_SET_METHOD(exports, "hasEvent", HasEvent);
    NODE_SET_METHOD(exports, "hasEvents", HasEvents);
    NODE_SET_METHOD(exports, "loadDollarTemplates", LoadDollarTemplates);
    NODE_SET_METHOD(exports, "pollEvent", PollEvent);
    NODE_SET_METHOD(exports, "pumpEvents", PumpEvents);
    NODE_SET_METHOD(exports, "quitRequested", QuitRequested);
    NODE_SET_METHOD(exports, "recordGesture", RecordGesture);
    NODE_SET_METHOD(exports, "registerEvents", RegisterEvents);
    NODE_SET_METHOD(exports, "saveAllDollarTemplates", SaveAllDollarTemplates);
    NODE_SET_METHOD(exports, "waitEvent", WaitEvent);
}
#include "deps.h"

// Not implemented:
// SDL_JoystickFromInstanceID - implemented in JS?

// Changes from SDL API:
// joystickGetDeviceGUID, joystickGetGUID
// These functions return a string rather than an array of bytes

namespace sdl2_bindings {

using namespace v8;

#define EXTRACT_JOYSTICK(ix) \
	UNWRAP(js, Joystick, args[ix])

METHOD(JoystickClose) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    js->close();
}

METHOD(JoystickCurrentPowerLevel) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    RETURN(MK_NUMBER(SDL_JoystickCurrentPowerLevel(js->joystick_)));
}

METHOD(JoystickEventState) {
    BEGIN();
    INTARG(state, 0);
    RETURN(MK_BOOL(SDL_JoystickEventState(state))); 
}

METHOD(JoystickGetAttached) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    RETURN(MK_BOOL(SDL_JoystickGetAttached(js->joystick_)));
}

METHOD(JoystickGetAxis) {
    BEGIN();
    EXTRACT_JOYSTICK(0);    
    INTARG(axis, 1);
    RETURN(MK_NUMBER(SDL_JoystickGetHat(js->joystick_, axis)));
}

METHOD(JoystickGetBall) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    INTARG(ball, 1);
    int dx, dy;
    int res = SDL_JoystickGetBall(js->joystick_, ball, &dx, &dy);
    if (res < 0) {
        THROW_SDL_ERROR();
    }
    auto out = MK_OBJECT();
    GET_CONTEXT();
    SET_KEY(out, SYM(dx), MK_NUMBER(dx));
    SET_KEY(out, SYM(dy), MK_NUMBER(dy));
    RETURN(out);
}

METHOD(JoystickGetButton) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    INTARG(button, 1);
    RETURN(MK_NUMBER(SDL_JoystickGetButton(js->joystick_, button)));
}

void getGUID(const FunctionCallbackInfo<Value>& args, Isolate *isolate, SDL_JoystickGUID *guid) {
    bool zero = true;
    for (int i = 0; i < 16; ++i) {
        if (guid->data[i] != 0) {
            zero = false;
            break;
        }
    }
    if (zero) {
        THROW_SDL_ERROR();
    } else {
        char buffer[33];
        SDL_JoystickGetGUIDString(*guid, buffer, 33);
        RETURN(MK_STRING(buffer));
    }
}

METHOD(JoystickGetDeviceGUID) {
    BEGIN();
    INTARG(deviceIndex, 0);
    auto guid = SDL_JoystickGetDeviceGUID(deviceIndex);
    getGUID(args, isolate, &guid);
}

METHOD(JoystickGetGUID) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    auto guid = SDL_JoystickGetGUID(js->joystick_);
    getGUID(args, isolate, &guid);
}

METHOD(JoystickGetHat) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    INTARG(hat, 1);
    RETURN(MK_NUMBER(SDL_JoystickGetHat(js->joystick_, hat)));
}

METHOD(JoystickInstanceID) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    RETURN(MK_NUMBER(SDL_JoystickInstanceID(js->joystick_)));
}

METHOD(JoystickName) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    RETURN(MK_STRING(SDL_JoystickName(js->joystick_)));
}

METHOD(JoystickNameForIndex) {
    BEGIN();
    INTARG(index, 0);
    RETURN(MK_STRING(SDL_JoystickNameForIndex(index)));
}

METHOD(JoystickNumAxes) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    RETURN(MK_NUMBER(SDL_JoystickNumAxes(js->joystick_)));
}

METHOD(JoystickNumBalls) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    RETURN(MK_NUMBER(SDL_JoystickNumBalls(js->joystick_)));
}

METHOD(JoystickNumButtons) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    RETURN(MK_NUMBER(SDL_JoystickNumButtons(js->joystick_)));
}

METHOD(JoystickNumHats) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    RETURN(MK_NUMBER(SDL_JoystickNumHats(js->joystick_)));
}

METHOD(JoystickOpen) {
    BEGIN();
    INTARG(index, 0);
    SDL_Joystick *joystick = SDL_JoystickOpen(index);
    if (joystick == nullptr) {
        THROW_SDL_ERROR();
    } else {
    	RETURN(Joystick::NewInstance(isolate, joystick));
    }
}

METHOD(JoystickUpdate) {
    SDL_JoystickUpdate();
}

METHOD(NumJoysticks) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_NumJoysticks()));
}

void InitJoystickFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "joystickClose", JoystickClose);
    NODE_SET_METHOD(exports, "joystickCurrentPowerLevel", JoystickCurrentPowerLevel);
    NODE_SET_METHOD(exports, "joystickEventState", JoystickEventState);
    NODE_SET_METHOD(exports, "joystickGetAttached", JoystickGetAttached);
    NODE_SET_METHOD(exports, "joystickGetAxis", JoystickGetAxis);
    NODE_SET_METHOD(exports, "joystickGetBall", JoystickGetBall);
    NODE_SET_METHOD(exports, "joystickGetButton", JoystickGetButton);
    NODE_SET_METHOD(exports, "joystickGetDeviceGUID", JoystickGetDeviceGUID);
    NODE_SET_METHOD(exports, "joystickGetGUID", JoystickGetGUID);
    NODE_SET_METHOD(exports, "joystickGetHat", JoystickGetHat);
    NODE_SET_METHOD(exports, "joystickInstanceID", JoystickInstanceID);
    NODE_SET_METHOD(exports, "joystickName", JoystickName);
    NODE_SET_METHOD(exports, "joystickNameForIndex", JoystickNameForIndex);
    NODE_SET_METHOD(exports, "joystickNumAxes", JoystickNumAxes);
    NODE_SET_METHOD(exports, "joystickNumBalls", JoystickNumBalls);
    NODE_SET_METHOD(exports, "joystickNumButtons", JoystickNumButtons);
    NODE_SET_METHOD(exports, "joystickNumHats", JoystickNumHats);
    NODE_SET_METHOD(exports, "joystickOpen", JoystickOpen);
    NODE_SET_METHOD(exports, "joystickUpdate", JoystickUpdate);
    NODE_SET_METHOD(exports, "numJoysticks", NumJoysticks);
}

}

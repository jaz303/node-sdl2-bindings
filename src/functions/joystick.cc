#include "deps.h"

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

// METHOD(JoystickFromInstanceID) {
//  // TODO
// }

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
    RETURN(MK_NUMBER(SDL_JoystickGetHat(js->joystick_, ball)));
}

METHOD(JoystickGetButton) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    INTARG(button, 1);
    RETURN(MK_NUMBER(SDL_JoystickGetButton(js->joystick_, button)));
}

// METHOD(JoystickGetDeviceGUID) {
//  BEGIN();
//  NARGS(1);
//  INTARG(deviceIndex, 0);
//  RETURN(MK_NUMBER(SDL_JoystickGetDeviceGUID(deviceIndex)));  
// }

// METHOD(JoystickGetGUID) {
//  // TODO
// }

// METHOD(JoystickGetGUIDFromString) {
//  // TODO
// }

// METHOD(JoystickGetGUIDString) {
//  // TODO
// }

METHOD(JoystickGetHat) {
    BEGIN();
    EXTRACT_JOYSTICK(0);
    INTARG(hat, 1);
    RETURN(MK_NUMBER(SDL_JoystickGetHat(js->joystick_, hat)));
}

METHOD(JoystickInstanceID) {
    BEGIN();
    RETURN(args[0]);
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
    // NODE_SET_METHOD(exports, "joystickFromInstanceID", JoystickFromInstanceID);
    NODE_SET_METHOD(exports, "joystickGetAttached", JoystickGetAttached);
    NODE_SET_METHOD(exports, "joystickGetAxis", JoystickGetAxis);
    NODE_SET_METHOD(exports, "joystickGetBall", JoystickGetBall);
    NODE_SET_METHOD(exports, "joystickGetButton", JoystickGetButton);
    // NODE_SET_METHOD(exports, "joystickGetDeviceGUID", JoystickGetDeviceGUID);
    // NODE_SET_METHOD(exports, "joystickGetGUID", JoystickGetGUID);
    // NODE_SET_METHOD(exports, "joystickGetGUIDFromString", JoystickGetGUIDFromString);
    // NODE_SET_METHOD(exports, "joystickGetGUIDString", JoystickGetGUIDString);
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

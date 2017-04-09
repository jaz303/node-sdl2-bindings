#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

METHOD(ImageInit) {
    BEGIN();
    INTARG(flags, 0);
    RETURN_INT(IMG_Init(flags));
}

METHOD(ImageQuit) {
    IMG_Quit();
}

METHOD(ImageLoad) {
    BEGIN();
    STRINGARG(file, 0);
    SDL_Surface *image = IMG_Load(*file);
    if (image == nullptr) {
        THROW(Error, IMG_GetError());
    } else {
        RETURN(Surface::NewInstance(isolate, image, true));
    }
}

METHOD(ImageGetError) {
    BEGIN();
    RETURN(MK_STRING(IMG_GetError()));
}

void InitExtImageFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "imageInit", ImageInit);
    NODE_SET_METHOD(exports, "imageQuit", ImageQuit);
    NODE_SET_METHOD(exports, "imageLoad", ImageLoad);
    NODE_SET_METHOD(exports, "imageGetError", ImageGetError);
}

}

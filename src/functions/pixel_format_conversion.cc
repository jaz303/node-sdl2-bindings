#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

// Convenience function; avoids allocating a JS PixelFormat object
METHOD(SurfaceMapRGB) {
    BEGIN();
    UNWRAP(surface, Surface, args[0]);
    UINT32ARG(r, 1);
    UINT32ARG(g, 2);
    UINT32ARG(b, 3);
    RETURN(MK_NUMBER(SDL_MapRGB(surface->surface_->format, r, g, b)));
}

// Convenience function; avoids allocating a JS PixelFormat object
METHOD(SurfaceMapRGBA) {
    BEGIN();
    UNWRAP(surface, Surface, args[0]);
    UINT32ARG(r, 1);
    UINT32ARG(g, 2);
    UINT32ARG(b, 3);
    UINT32ARG(a, 4);
    RETURN(MK_NUMBER(SDL_MapRGBA(surface->surface_->format, r, g, b, a)));
}

void InitPixelFormatConversionFunctions(Local<Object> exports) {
 	NODE_SET_METHOD(exports, "surfaceMapRGB", SurfaceMapRGB);
 	NODE_SET_METHOD(exports, "surfaceMapRGBA", SurfaceMapRGBA);
}

}

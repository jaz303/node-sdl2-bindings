#include "deps.h"

namespace sdl2_bindings {

using namespace v8;

METHOD(GetCPUCacheLineSize) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetCPUCacheLineSize()));
}

METHOD(GetCPUCount) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetCPUCount()));
}

METHOD(GetSystemRAM) {
    BEGIN();
    RETURN(MK_NUMBER(SDL_GetSystemRAM()));
}

METHOD(Has3DNow) {
    BEGIN();
    RETURN(MK_BOOL(SDL_Has3DNow()));
}

METHOD(HasAVX) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasAVX()));
}

METHOD(HasAVX2) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasAVX2()));
}

METHOD(HasAltiVec) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasAltiVec()));
}

METHOD(HasMMX) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasMMX()));
}

METHOD(HasRDTSC) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasRDTSC()));
}

METHOD(HasSSE) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasSSE()));
}

METHOD(HasSSE2) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasSSE2()));
}

METHOD(HasSSE3) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasSSE3()));
}

METHOD(HasSSE41) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasSSE41()));
}

METHOD(HasSSE42) {
    BEGIN();
    RETURN(MK_BOOL(SDL_HasSSE42()));
}

void InitCPUFeatureDetectionFunctions(Local<Object> exports) {
    NODE_SET_METHOD(exports, "getCPUCacheLineSize", GetCPUCacheLineSize);
    NODE_SET_METHOD(exports, "getCPUCount", GetCPUCount);
    NODE_SET_METHOD(exports, "getSystemRAM", GetSystemRAM);
    NODE_SET_METHOD(exports, "has3DNow", Has3DNow);
    NODE_SET_METHOD(exports, "hasAVX", HasAVX);
    NODE_SET_METHOD(exports, "hasAVX2", HasAVX2);
    NODE_SET_METHOD(exports, "hasAltiVec", HasAltiVec);
    NODE_SET_METHOD(exports, "hasMMX", HasMMX);
    NODE_SET_METHOD(exports, "hasRDTSC", HasRDTSC);
    NODE_SET_METHOD(exports, "hasSSE", HasSSE);
    NODE_SET_METHOD(exports, "hasSSE2", HasSSE2);
    NODE_SET_METHOD(exports, "hasSSE3", HasSSE3);
    NODE_SET_METHOD(exports, "hasSSE41", HasSSE41);
    NODE_SET_METHOD(exports, "hasSSE42", HasSSE42);
}

}

// SDL_GetClosestDisplayMode
// SDL_GetCurrentDisplayMode
// SDL_GetCurrentVideoDriver
// SDL_GetDesktopDisplayMode
// SDL_GetDisplayBounds
// SDL_GetDisplayDPI
// SDL_GetDisplayMode
// SDL_GetDisplayName
// SDL_GetDisplayUsableBounds

// SDL_GetGrabbedWindow
// SDL_GetNumDisplayModes
// SDL_GetNumVideoDisplays
// SDL_GetNumVideoDrivers
// SDL_GetVideoDriver

METHOD(CreateWindow) {
    BEGIN();

    NARGS(6);
    STRINGARG(title, 0);
    INTARG(x, 1);
    INTARG(y, 2);
    INTARG(w, 3);
    INTARG(h, 4);
    UINT32ARG(flags, 5);

    SDL_Window *win = SDL_CreateWindow(*title, x, y, w, h, flags);
    if (!win) {
    	THROW_SDL_ERROR();
    }

    RETURN(Window::NewInstance(isolate, win));
}

METHOD(DisableScreenSaver) {
    SDL_DisableScreenSaver();
}

METHOD(EnableScreenSaver) {
    SDL_EnableScreenSaver();
}

METHOD(IsScreenSaverEnabled) {
    BEGIN();
    RETURN(MK_BOOL(SDL_IsScreenSaverEnabled() == SDL_TRUE));
}

METHOD(VideoInit) {
	BEGIN();
	STRINGARG(driverName, 0);
	auto res = SDL_VideoInit(*driverName);
	if (res != 0) {
		THROW_SDL_ERROR();
	}
}

METHOD(VideoQuit) {
	SDL_VideoQuit();
}

METHOD(ShowMessageBox) {
	showMessageBox(args, NULL);
}

METHOD(ShowSimpleMessageBox) {
	showSimpleMessageBox(args, NULL);
}

void initDisplay(Local<Object> exports) {
    NODE_SET_METHOD(exports, "createWindow", CreateWindow);
    NODE_SET_METHOD(exports, "disableScreensaver", DisableScreenSaver);
    NODE_SET_METHOD(exports, "enableScreensaver", EnableScreenSaver);
    NODE_SET_METHOD(exports, "isScreenSaverEnabled", IsScreenSaverEnabled);
    NODE_SET_METHOD(exports, "videoInit", VideoInit);
    NODE_SET_METHOD(exports, "videoQuit", VideoQuit);
    NODE_SET_METHOD(exports, "showMessageBox", ShowMessageBox);
    NODE_SET_METHOD(exports, "showSimpleMessageBox", ShowSimpleMessageBox);
}
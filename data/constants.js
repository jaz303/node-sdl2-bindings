const S = process.stdout;

S.write('#include "deps.h"\n\n');
S.write('namespace sdl2_bindings {\n\n');
S.write('using namespace v8;\n');
S.write('void InitConstants(Local<Object> exports) {\n');
S.write('    auto isolate = exports->GetIsolate();\n');
S.write('    auto attrs = static_cast<v8::PropertyAttribute>(v8::ReadOnly | v8::DontDelete);\n');
S.write('    auto ctx = isolate->GetCurrentContext();\n');

writeConstants({
values: [
		// Hints
		'SDL_HINT_ACCELEROMETER_AS_JOYSTICK',
		'SDL_HINT_ANDROID_APK_EXPANSION_MAIN_FILE_VERSION',
		'SDL_HINT_ANDROID_APK_EXPANSION_PATCH_FILE_VERSION',
		'SDL_HINT_ANDROID_SEPARATE_MOUSE_AND_TOUCH',
		// 'SDL_HINT_APPLE_TV_CONTROLLER_UI_EVENTS',
		// 'SDL_HINT_APPLE_TV_REMOTE_ALLOW_ROTATION',
		// 'SDL_HINT_BMP_SAVE_LEGACY_FORMAT',
		'SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT',
		'SDL_HINT_FRAMEBUFFER_ACCELERATION',
		'SDL_HINT_GAMECONTROLLERCONFIG',
		'SDL_HINT_GRAB_KEYBOARD',
		'SDL_HINT_IDLE_TIMER_DISABLED',
		'SDL_HINT_IME_INTERNAL_EDITING',
		'SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS',
		'SDL_HINT_MAC_BACKGROUND_APP',
		'SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK',
		// 'SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH',
		'SDL_HINT_MOUSE_RELATIVE_MODE_WARP',
		'SDL_HINT_NO_SIGNAL_HANDLERS',
		'SDL_HINT_ORIENTATIONS',
		'SDL_HINT_RENDER_DIRECT3D11_DEBUG',
		'SDL_HINT_RENDER_DIRECT3D_THREADSAFE',
		'SDL_HINT_RENDER_DRIVER',
		'SDL_HINT_RENDER_OPENGL_SHADERS',
		'SDL_HINT_RENDER_SCALE_QUALITY',
		'SDL_HINT_RENDER_VSYNC',
		// 'SDL_HINT_RPI_VIDEO_LAYER',
		'SDL_HINT_THREAD_STACK_SIZE',
		'SDL_HINT_TIMER_RESOLUTION',
		'SDL_HINT_VIDEO_ALLOW_SCREENSAVER',
		'SDL_HINT_VIDEO_HIGHDPI_DISABLED',
		'SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES',
		'SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS',
		'SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT',
		'SDL_HINT_VIDEO_WIN_D3DCOMPILER',
		'SDL_HINT_VIDEO_X11_NET_WM_PING',
		'SDL_HINT_VIDEO_X11_XINERAMA',
		'SDL_HINT_VIDEO_X11_XRANDR',
		'SDL_HINT_VIDEO_X11_XVIDMODE',
		// 'SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING',
		'SDL_HINT_WINDOWS_ENABLE_MESSAGELOOP',
		'SDL_HINT_WINDOWS_NO_CLOSE_ON_ALT_F4',
		'SDL_HINT_WINDOW_FRAME_USABLE_WHILE_CURSOR_HIDDEN',
		'SDL_HINT_WINRT_HANDLE_BACK_BUTTON',
		'SDL_HINT_WINRT_PRIVACY_POLICY_LABEL',
		'SDL_HINT_WINRT_PRIVACY_POLICY_URL',
		'SDL_HINT_XINPUT_ENABLED',
		'SDL_HINT_XINPUT_USE_OLD_JOYSTICK_MAPPING',
	],
	filter: (n) => n.replace(/^SDL_/, ''),
	template: (n) => "v8::String::NewFromUtf8(isolate, \"" + n + "\")"
});

writeConstants({
	values: [

		'SDL_WINDOWPOS_CENTERED',
		'SDL_WINDOWPOS_UNDEFINED',

		'SDL_ASSERTION_RETRY',
		'SDL_ASSERTION_BREAK',
		'SDL_ASSERTION_ABORT',
		'SDL_ASSERTION_IGNORE',
		'SDL_ASSERTION_ALWAYS_IGNORE',

		// Audio formats
		'AUDIO_S8',
		'AUDIO_U8',
		'AUDIO_S16LSB',
		'AUDIO_S16MSB',
		'AUDIO_S16SYS',
		'AUDIO_S16',
		'AUDIO_U16LSB',
		'AUDIO_U16MSB',
		'AUDIO_U16SYS',
		'AUDIO_U16',
		'AUDIO_S32LSB',
		'AUDIO_S32MSB',
		'AUDIO_S32SYS',
		'AUDIO_S32',
		'AUDIO_F32LSB',
		'AUDIO_F32MSB',
		'AUDIO_F32SYS',
		'AUDIO_F32',

		'IMG_INIT_JPG',
		'IMG_INIT_PNG',
		'IMG_INIT_TIF',

		'SDL_AUDIO_STOPPED',
		'SDL_AUDIO_PLAYING',
		'SDL_AUDIO_PAUSED',

		'SDL_BLENDMODE_NONE',
		'SDL_BLENDMODE_BLEND',
		'SDL_BLENDMODE_ADD',
		'SDL_BLENDMODE_MOD',

		// Events
		'SDL_FIRSTEVENT',
		'SDL_QUIT',
		'SDL_APP_TERMINATING',
		'SDL_APP_LOWMEMORY',
		'SDL_APP_WILLENTERBACKGROUND',
		'SDL_APP_DIDENTERBACKGROUND',
		'SDL_APP_WILLENTERFOREGROUND',
		'SDL_APP_DIDENTERFOREGROUND',
		'SDL_WINDOWEVENT',
		'SDL_SYSWMEVENT',
		'SDL_KEYDOWN',
		'SDL_KEYUP',
		'SDL_TEXTEDITING',
		'SDL_TEXTINPUT',
		'SDL_KEYMAPCHANGED',
		'SDL_MOUSEMOTION',
		'SDL_MOUSEBUTTONDOWN',
		'SDL_MOUSEBUTTONUP',
		'SDL_MOUSEWHEEL',

		// Subsytem initialisation
		'SDL_INIT_TIMER',
		'SDL_INIT_AUDIO',
		'SDL_INIT_VIDEO',
		'SDL_INIT_JOYSTICK',
		'SDL_INIT_HAPTIC',
		'SDL_INIT_GAMECONTROLLER',
		'SDL_INIT_EVENTS',
		'SDL_INIT_EVERYTHING',
		'SDL_INIT_NOPARACHUTE',

		'SDL_JOYAXISMOTION',
		'SDL_JOYBALLMOTION',
		'SDL_JOYHATMOTION',
		'SDL_JOYBUTTONDOWN',
		'SDL_JOYBUTTONUP',
		'SDL_JOYDEVICEADDED',
		'SDL_JOYDEVICEREMOVED',
		'SDL_CONTROLLERAXISMOTION',
		'SDL_CONTROLLERBUTTONDOWN',
		'SDL_CONTROLLERBUTTONUP',
		'SDL_CONTROLLERDEVICEADDED',
		'SDL_CONTROLLERDEVICEREMOVED',
		'SDL_CONTROLLERDEVICEREMAPPED',
		'SDL_FINGERDOWN',
		'SDL_FINGERUP',
		'SDL_FINGERMOTION',
		'SDL_DOLLARGESTURE',
		'SDL_DOLLARRECORD',
		'SDL_MULTIGESTURE',
		'SDL_CLIPBOARDUPDATE',
		'SDL_DROPFILE',
		// 'SDL_DROPTEXT',
		// 'SDL_DROPBEGIN',
		// 'SDL_DROPCOMPLETE',
		'SDL_AUDIODEVICEADDED',
		'SDL_AUDIODEVICEREMOVED',
		'SDL_RENDER_TARGETS_RESET',
		'SDL_RENDER_DEVICE_RESET',
		'SDL_USEREVENT',
		'SDL_LASTEVENT',
		'SDL_GL_RED_SIZE',
		'SDL_GL_GREEN_SIZE',
		'SDL_GL_BLUE_SIZE',
		'SDL_GL_ALPHA_SIZE',
		'SDL_GL_BUFFER_SIZE',
		'SDL_GL_DOUBLEBUFFER',
		'SDL_GL_DEPTH_SIZE',
		'SDL_GL_STENCIL_SIZE',
		'SDL_GL_ACCUM_RED_SIZE',
		'SDL_GL_ACCUM_GREEN_SIZE',
		'SDL_GL_ACCUM_BLUE_SIZE',
		'SDL_GL_ACCUM_ALPHA_SIZE',
		'SDL_GL_STEREO',
		'SDL_GL_MULTISAMPLEBUFFERS',
		'SDL_GL_MULTISAMPLESAMPLES',
		'SDL_GL_ACCELERATED_VISUAL',
		'SDL_GL_RETAINED_BACKING',
		'SDL_GL_CONTEXT_MAJOR_VERSION',
		'SDL_GL_CONTEXT_MINOR_VERSION',
		'SDL_GL_CONTEXT_FLAGS',
		'SDL_GL_CONTEXT_PROFILE_MASK',
		'SDL_GL_SHARE_WITH_CURRENT_CONTEXT',
		'SDL_GL_FRAMEBUFFER_SRGB_CAPABLE',
		'SDL_GL_CONTEXT_RELEASE_BEHAVIOR',
		'SDL_GL_CONTEXT_EGL',
		'SDL_GL_CONTEXT_DEBUG_FLAG',
		'SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG',
		'SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG',
		'SDL_GL_CONTEXT_RESET_ISOLATION_FLAG',
		'SDL_GL_CONTEXT_PROFILE_CORE',
		'SDL_GL_CONTEXT_PROFILE_COMPATIBILITY',
		'SDL_GL_CONTEXT_PROFILE_ES',
		'SDL_CONTROLLER_AXIS_INVALID',
		'SDL_CONTROLLER_AXIS_LEFTX',
		'SDL_CONTROLLER_AXIS_LEFTY',
		'SDL_CONTROLLER_AXIS_RIGHTX',
		'SDL_CONTROLLER_AXIS_RIGHTY',
		'SDL_CONTROLLER_AXIS_TRIGGERLEFT',
		'SDL_CONTROLLER_AXIS_TRIGGERRIGHT',
		'SDL_CONTROLLER_AXIS_MAX',
		'SDL_CONTROLLER_BUTTON_INVALID',
		'SDL_CONTROLLER_BUTTON_A',
		'SDL_CONTROLLER_BUTTON_B',
		'SDL_CONTROLLER_BUTTON_X',
		'SDL_CONTROLLER_BUTTON_Y',
		'SDL_CONTROLLER_BUTTON_BACK',
		'SDL_CONTROLLER_BUTTON_GUIDE',
		'SDL_CONTROLLER_BUTTON_START',
		'SDL_CONTROLLER_BUTTON_LEFTSTICK',
		'SDL_CONTROLLER_BUTTON_RIGHTSTICK',
		'SDL_CONTROLLER_BUTTON_LEFTSHOULDER',
		'SDL_CONTROLLER_BUTTON_RIGHTSHOULDER',
		'SDL_CONTROLLER_BUTTON_DPAD_UP',
		'SDL_CONTROLLER_BUTTON_DPAD_DOWN',
		'SDL_CONTROLLER_BUTTON_DPAD_LEFT',
		'SDL_CONTROLLER_BUTTON_DPAD_RIGHT',
		'SDL_CONTROLLER_BUTTON_MAX',
		'SDL_HINT_DEFAULT',
		'SDL_HINT_NORMAL',
		'SDL_HINT_OVERRIDE',
		'SDL_HITTEST_NORMAL',
		'SDL_HITTEST_DRAGGABLE',
		'SDL_HITTEST_RESIZE_TOPLEFT',
		'SDL_HITTEST_RESIZE_TOP',
		'SDL_HITTEST_RESIZE_TOPRIGHT',
		'SDL_HITTEST_RESIZE_RIGHT',
		'SDL_HITTEST_RESIZE_BOTTOMRIGHT',
		'SDL_HITTEST_RESIZE_BOTTOM',
		'SDL_HITTEST_RESIZE_BOTTOMLEFT',
		'SDL_HITTEST_RESIZE_LEFT',
		'SDL_JOYSTICK_POWER_UNKNOWN',
		'SDL_JOYSTICK_POWER_EMPTY',
		'SDL_JOYSTICK_POWER_LOW',
		'SDL_JOYSTICK_POWER_MEDIUM',
		'SDL_JOYSTICK_POWER_FULL',
		'SDL_JOYSTICK_POWER_WIRED',
		'SDL_JOYSTICK_POWER_MAX',
		'SDL_SCANCODE_0',
		'SDLK_0',
		'SDL_SCANCODE_1',
		'SDLK_1',
		'SDL_SCANCODE_2',
		'SDLK_2',
		'SDL_SCANCODE_3',
		'SDLK_3',
		'SDL_SCANCODE_4',
		'SDLK_4',
		'SDL_SCANCODE_5',
		'SDLK_5',
		'SDL_SCANCODE_6',
		'SDLK_6',
		'SDL_SCANCODE_7',
		'SDLK_7',
		'SDL_SCANCODE_8',
		'SDLK_8',
		'SDL_SCANCODE_9',
		'SDLK_9',
		'SDL_SCANCODE_A',
		'SDLK_a',
		'SDL_SCANCODE_AC_BACK',
		'SDLK_AC_BACK',
		'SDL_SCANCODE_AC_BOOKMARKS',
		'SDLK_AC_BOOKMARKS',
		'SDL_SCANCODE_AC_FORWARD',
		'SDLK_AC_FORWARD',
		'SDL_SCANCODE_AC_HOME',
		'SDLK_AC_HOME',
		'SDL_SCANCODE_AC_REFRESH',
		'SDLK_AC_REFRESH',
		'SDL_SCANCODE_AC_SEARCH',
		'SDLK_AC_SEARCH',
		'SDL_SCANCODE_AC_STOP',
		'SDLK_AC_STOP',
		'SDL_SCANCODE_AGAIN',
		'SDLK_AGAIN',
		'SDL_SCANCODE_ALTERASE',
		'SDLK_ALTERASE',
		'SDL_SCANCODE_APOSTROPHE',
		'SDLK_QUOTE',
		'SDL_SCANCODE_APPLICATION',
		'SDLK_APPLICATION',
		'SDL_SCANCODE_AUDIOMUTE',
		'SDLK_AUDIOMUTE',
		'SDL_SCANCODE_AUDIONEXT',
		'SDLK_AUDIONEXT',
		'SDL_SCANCODE_AUDIOPLAY',
		'SDLK_AUDIOPLAY',
		'SDL_SCANCODE_AUDIOPREV',
		'SDLK_AUDIOPREV',
		'SDL_SCANCODE_AUDIOSTOP',
		'SDLK_AUDIOSTOP',
		'SDL_SCANCODE_B',
		'SDLK_b',
		'SDL_SCANCODE_BACKSLASH',
		'SDLK_BACKSLASH',
		'SDL_SCANCODE_BACKSPACE',
		'SDLK_BACKSPACE',
		'SDL_SCANCODE_BRIGHTNESSDOWN',
		'SDLK_BRIGHTNESSDOWN',
		'SDL_SCANCODE_BRIGHTNESSUP',
		'SDLK_BRIGHTNESSUP',
		'SDL_SCANCODE_C',
		'SDLK_c',
		'SDL_SCANCODE_CALCULATOR',
		'SDLK_CALCULATOR',
		'SDL_SCANCODE_CANCEL',
		'SDLK_CANCEL',
		'SDL_SCANCODE_CAPSLOCK',
		'SDLK_CAPSLOCK',
		'SDL_SCANCODE_CLEAR',
		'SDLK_CLEAR',
		'SDL_SCANCODE_CLEARAGAIN',
		'SDLK_CLEARAGAIN',
		'SDL_SCANCODE_COMMA',
		'SDLK_COMMA',
		'SDL_SCANCODE_COMPUTER',
		'SDLK_COMPUTER',
		'SDL_SCANCODE_COPY',
		'SDLK_COPY',
		'SDL_SCANCODE_CRSEL',
		'SDLK_CRSEL',
		'SDL_SCANCODE_CURRENCYSUBUNIT',
		'SDLK_CURRENCYSUBUNIT',
		'SDL_SCANCODE_CURRENCYUNIT',
		'SDLK_CURRENCYUNIT',
		'SDL_SCANCODE_CUT',
		'SDLK_CUT',
		'SDL_SCANCODE_D',
		'SDLK_d',
		'SDL_SCANCODE_DECIMALSEPARATOR',
		'SDLK_DECIMALSEPARATOR',
		'SDL_SCANCODE_DELETE',
		'SDLK_DELETE',
		'SDL_SCANCODE_DISPLAYSWITCH',
		'SDLK_DISPLAYSWITCH',
		'SDL_SCANCODE_DOWN',
		'SDLK_DOWN',
		'SDL_SCANCODE_E',
		'SDLK_e',
		'SDL_SCANCODE_EJECT',
		'SDLK_EJECT',
		'SDL_SCANCODE_END',
		'SDLK_END',
		'SDL_SCANCODE_EQUALS',
		'SDLK_EQUALS',
		'SDL_SCANCODE_ESCAPE',
		'SDLK_ESCAPE',
		'SDL_SCANCODE_EXECUTE',
		'SDLK_EXECUTE',
		'SDL_SCANCODE_EXSEL',
		'SDLK_EXSEL',
		'SDL_SCANCODE_F',
		'SDLK_f',
		'SDL_SCANCODE_F1',
		'SDLK_F1',
		'SDL_SCANCODE_F10',
		'SDLK_F10',
		'SDL_SCANCODE_F11',
		'SDLK_F11',
		'SDL_SCANCODE_F12',
		'SDLK_F12',
		'SDL_SCANCODE_F13',
		'SDLK_F13',
		'SDL_SCANCODE_F14',
		'SDLK_F14',
		'SDL_SCANCODE_F15',
		'SDLK_F15',
		'SDL_SCANCODE_F16',
		'SDLK_F16',
		'SDL_SCANCODE_F17',
		'SDLK_F17',
		'SDL_SCANCODE_F18',
		'SDLK_F18',
		'SDL_SCANCODE_F19',
		'SDLK_F19',
		'SDL_SCANCODE_F2',
		'SDLK_F2',
		'SDL_SCANCODE_F20',
		'SDLK_F20',
		'SDL_SCANCODE_F21',
		'SDLK_F21',
		'SDL_SCANCODE_F22',
		'SDLK_F22',
		'SDL_SCANCODE_F23',
		'SDLK_F23',
		'SDL_SCANCODE_F24',
		'SDLK_F24',
		'SDL_SCANCODE_F3',
		'SDLK_F3',
		'SDL_SCANCODE_F4',
		'SDLK_F4',
		'SDL_SCANCODE_F5',
		'SDLK_F5',
		'SDL_SCANCODE_F6',
		'SDLK_F6',
		'SDL_SCANCODE_F7',
		'SDLK_F7',
		'SDL_SCANCODE_F8',
		'SDLK_F8',
		'SDL_SCANCODE_F9',
		'SDLK_F9',
		'SDL_SCANCODE_FIND',
		'SDLK_FIND',
		'SDL_SCANCODE_G',
		'SDLK_g',
		'SDL_SCANCODE_GRAVE',
		'SDLK_BACKQUOTE',
		'SDL_SCANCODE_H',
		'SDLK_h',
		'SDL_SCANCODE_HELP',
		'SDLK_HELP',
		'SDL_SCANCODE_HOME',
		'SDLK_HOME',
		'SDL_SCANCODE_I',
		'SDLK_i',
		'SDL_SCANCODE_INSERT',
		'SDLK_INSERT',
		'SDL_SCANCODE_J',
		'SDLK_j',
		'SDL_SCANCODE_K',
		'SDLK_k',
		'SDL_SCANCODE_KBDILLUMDOWN',
		'SDLK_KBDILLUMDOWN',
		'SDL_SCANCODE_KBDILLUMTOGGLE',
		'SDLK_KBDILLUMTOGGLE',
		'SDL_SCANCODE_KBDILLUMUP',
		'SDLK_KBDILLUMUP',
		'SDL_SCANCODE_KP_0',
		'SDLK_KP_0',
		'SDL_SCANCODE_KP_00',
		'SDLK_KP_00',
		'SDL_SCANCODE_KP_000',
		'SDLK_KP_000',
		'SDL_SCANCODE_KP_1',
		'SDLK_KP_1',
		'SDL_SCANCODE_KP_2',
		'SDLK_KP_2',
		'SDL_SCANCODE_KP_3',
		'SDLK_KP_3',
		'SDL_SCANCODE_KP_4',
		'SDLK_KP_4',
		'SDL_SCANCODE_KP_5',
		'SDLK_KP_5',
		'SDL_SCANCODE_KP_6',
		'SDLK_KP_6',
		'SDL_SCANCODE_KP_7',
		'SDLK_KP_7',
		'SDL_SCANCODE_KP_8',
		'SDLK_KP_8',
		'SDL_SCANCODE_KP_9',
		'SDLK_KP_9',
		'SDL_SCANCODE_KP_A',
		'SDLK_KP_A',
		'SDL_SCANCODE_KP_AMPERSAND',
		'SDLK_KP_AMPERSAND',
		'SDL_SCANCODE_KP_AT',
		'SDLK_KP_AT',
		'SDL_SCANCODE_KP_B',
		'SDLK_KP_B',
		'SDL_SCANCODE_KP_BACKSPACE',
		'SDLK_KP_BACKSPACE',
		'SDL_SCANCODE_KP_BINARY',
		'SDLK_KP_BINARY',
		'SDL_SCANCODE_KP_C',
		'SDLK_KP_C',
		'SDL_SCANCODE_KP_CLEAR',
		'SDLK_KP_CLEAR',
		'SDL_SCANCODE_KP_CLEARENTRY',
		'SDLK_KP_CLEARENTRY',
		'SDL_SCANCODE_KP_COLON',
		'SDLK_KP_COLON',
		'SDL_SCANCODE_KP_COMMA',
		'SDLK_KP_COMMA',
		'SDL_SCANCODE_KP_D',
		'SDLK_KP_D',
		'SDL_SCANCODE_KP_DBLAMPERSAND',
		'SDLK_KP_DBLAMPERSAND',
		'SDL_SCANCODE_KP_DBLVERTICALBAR',
		'SDLK_KP_DBLVERTICALBAR',
		'SDL_SCANCODE_KP_DECIMAL',
		'SDLK_KP_DECIMAL',
		'SDL_SCANCODE_KP_DIVIDE',
		'SDLK_KP_DIVIDE',
		'SDL_SCANCODE_KP_E',
		'SDLK_KP_E',
		'SDL_SCANCODE_KP_ENTER',
		'SDLK_KP_ENTER',
		'SDL_SCANCODE_KP_EQUALS',
		'SDLK_KP_EQUALS',
		'SDL_SCANCODE_KP_EQUALSAS400',
		'SDLK_KP_EQUALSAS400',
		'SDL_SCANCODE_KP_EXCLAM',
		'SDLK_KP_EXCLAM',
		'SDL_SCANCODE_KP_F',
		'SDLK_KP_F',
		'SDL_SCANCODE_KP_GREATER',
		'SDLK_KP_GREATER',
		'SDL_SCANCODE_KP_HASH',
		'SDLK_KP_HASH',
		'SDL_SCANCODE_KP_HEXADECIMAL',
		'SDLK_KP_HEXADECIMAL',
		'SDL_SCANCODE_KP_LEFTBRACE',
		'SDLK_KP_LEFTBRACE',
		'SDL_SCANCODE_KP_LEFTPAREN',
		'SDLK_KP_LEFTPAREN',
		'SDL_SCANCODE_KP_LESS',
		'SDLK_KP_LESS',
		'SDL_SCANCODE_KP_MEMADD',
		'SDLK_KP_MEMADD',
		'SDL_SCANCODE_KP_MEMCLEAR',
		'SDLK_KP_MEMCLEAR',
		'SDL_SCANCODE_KP_MEMDIVIDE',
		'SDLK_KP_MEMDIVIDE',
		'SDL_SCANCODE_KP_MEMMULTIPLY',
		'SDLK_KP_MEMMULTIPLY',
		'SDL_SCANCODE_KP_MEMRECALL',
		'SDLK_KP_MEMRECALL',
		'SDL_SCANCODE_KP_MEMSTORE',
		'SDLK_KP_MEMSTORE',
		'SDL_SCANCODE_KP_MEMSUBTRACT',
		'SDLK_KP_MEMSUBTRACT',
		'SDL_SCANCODE_KP_MINUS',
		'SDLK_KP_MINUS',
		'SDL_SCANCODE_KP_MULTIPLY',
		'SDLK_KP_MULTIPLY',
		'SDL_SCANCODE_KP_OCTAL',
		'SDLK_KP_OCTAL',
		'SDL_SCANCODE_KP_PERCENT',
		'SDLK_KP_PERCENT',
		'SDL_SCANCODE_KP_PERIOD',
		'SDLK_KP_PERIOD',
		'SDL_SCANCODE_KP_PLUS',
		'SDLK_KP_PLUS',
		'SDL_SCANCODE_KP_PLUSMINUS',
		'SDLK_KP_PLUSMINUS',
		'SDL_SCANCODE_KP_POWER',
		'SDLK_KP_POWER',
		'SDL_SCANCODE_KP_RIGHTBRACE',
		'SDLK_KP_RIGHTBRACE',
		'SDL_SCANCODE_KP_RIGHTPAREN',
		'SDLK_KP_RIGHTPAREN',
		'SDL_SCANCODE_KP_SPACE',
		'SDLK_KP_SPACE',
		'SDL_SCANCODE_KP_TAB',
		'SDLK_KP_TAB',
		'SDL_SCANCODE_KP_VERTICALBAR',
		'SDLK_KP_VERTICALBAR',
		'SDL_SCANCODE_KP_XOR',
		'SDLK_KP_XOR',
		'SDL_SCANCODE_L',
		'SDLK_l',
		'SDL_SCANCODE_LALT',
		'SDLK_LALT',
		'SDL_SCANCODE_LCTRL',
		'SDLK_LCTRL',
		'SDL_SCANCODE_LEFT',
		'SDLK_LEFT',
		'SDL_SCANCODE_LEFTBRACKET',
		'SDLK_LEFTBRACKET',
		'SDL_SCANCODE_LGUI',
		'SDLK_LGUI',
		'SDL_SCANCODE_LSHIFT',
		'SDLK_LSHIFT',
		'SDL_SCANCODE_M',
		'SDLK_m',
		'SDL_SCANCODE_MAIL',
		'SDLK_MAIL',
		'SDL_SCANCODE_MEDIASELECT',
		'SDLK_MEDIASELECT',
		'SDL_SCANCODE_MENU',
		'SDLK_MENU',
		'SDL_SCANCODE_MINUS',
		'SDLK_MINUS',
		'SDL_SCANCODE_MODE',
		'SDLK_MODE',
		'SDL_SCANCODE_MUTE',
		'SDLK_MUTE',
		'SDL_SCANCODE_N',
		'SDLK_n',
		'SDL_SCANCODE_NUMLOCKCLEAR',
		'SDLK_NUMLOCKCLEAR',
		'SDL_SCANCODE_O',
		'SDLK_o',
		'SDL_SCANCODE_OPER',
		'SDLK_OPER',
		'SDL_SCANCODE_OUT',
		'SDLK_OUT',
		'SDL_SCANCODE_P',
		'SDLK_p',
		'SDL_SCANCODE_PAGEDOWN',
		'SDLK_PAGEDOWN',
		'SDL_SCANCODE_PAGEUP',
		'SDLK_PAGEUP',
		'SDL_SCANCODE_PASTE',
		'SDLK_PASTE',
		'SDL_SCANCODE_PAUSE',
		'SDLK_PAUSE',
		'SDL_SCANCODE_PERIOD',
		'SDLK_PERIOD',
		'SDL_SCANCODE_POWER',
		'SDLK_POWER',
		'SDL_SCANCODE_PRINTSCREEN',
		'SDLK_PRINTSCREEN',
		'SDL_SCANCODE_PRIOR',
		'SDLK_PRIOR',
		'SDL_SCANCODE_Q',
		'SDLK_q',
		'SDL_SCANCODE_R',
		'SDLK_r',
		'SDL_SCANCODE_RALT',
		'SDLK_RALT',
		'SDL_SCANCODE_RCTRL',
		'SDLK_RCTRL',
		'SDL_SCANCODE_RETURN',
		'SDLK_RETURN',
		'SDL_SCANCODE_RETURN2',
		'SDLK_RETURN2',
		'SDL_SCANCODE_RGUI',
		'SDLK_RGUI',
		'SDL_SCANCODE_RIGHT',
		'SDLK_RIGHT',
		'SDL_SCANCODE_RIGHTBRACKET',
		'SDLK_RIGHTBRACKET',
		'SDL_SCANCODE_RSHIFT',
		'SDLK_RSHIFT',
		'SDL_SCANCODE_S',
		'SDLK_s',
		'SDL_SCANCODE_SCROLLLOCK',
		'SDLK_SCROLLLOCK',
		'SDL_SCANCODE_SELECT',
		'SDLK_SELECT',
		'SDL_SCANCODE_SEMICOLON',
		'SDLK_SEMICOLON',
		'SDL_SCANCODE_SEPARATOR',
		'SDLK_SEPARATOR',
		'SDL_SCANCODE_SLASH',
		'SDLK_SLASH',
		'SDL_SCANCODE_SLEEP',
		'SDLK_SLEEP',
		'SDL_SCANCODE_SPACE',
		'SDLK_SPACE',
		'SDL_SCANCODE_STOP',
		'SDLK_STOP',
		'SDL_SCANCODE_SYSREQ',
		'SDLK_SYSREQ',
		'SDL_SCANCODE_T',
		'SDLK_t',
		'SDL_SCANCODE_TAB',
		'SDLK_TAB',
		'SDL_SCANCODE_THOUSANDSSEPARATOR',
		'SDLK_THOUSANDSSEPARATOR',
		'SDL_SCANCODE_U',
		'SDLK_u',
		'SDL_SCANCODE_UNDO',
		'SDLK_UNDO',
		'SDL_SCANCODE_UNKNOWN',
		'SDLK_UNKNOWN',
		'SDL_SCANCODE_UP',
		'SDLK_UP',
		'SDL_SCANCODE_V',
		'SDLK_v',
		'SDL_SCANCODE_VOLUMEDOWN',
		'SDLK_VOLUMEDOWN',
		'SDL_SCANCODE_VOLUMEUP',
		'SDLK_VOLUMEUP',
		'SDL_SCANCODE_W',
		'SDLK_w',
		'SDL_SCANCODE_WWW',
		'SDLK_WWW',
		'SDL_SCANCODE_X',
		'SDLK_x',
		'SDL_SCANCODE_Y',
		'SDLK_y',
		'SDL_SCANCODE_Z',
		'SDLK_z',
		'SDL_SCANCODE_INTERNATIONAL1',
		'SDL_SCANCODE_INTERNATIONAL2',
		'SDL_SCANCODE_INTERNATIONAL3',
		'SDL_SCANCODE_INTERNATIONAL4',
		'SDL_SCANCODE_INTERNATIONAL5',
		'SDL_SCANCODE_INTERNATIONAL6',
		'SDL_SCANCODE_INTERNATIONAL7',
		'SDL_SCANCODE_INTERNATIONAL8',
		'SDL_SCANCODE_INTERNATIONAL9',
		'SDL_SCANCODE_LANG1',
		'SDL_SCANCODE_LANG2',
		'SDL_SCANCODE_LANG3',
		'SDL_SCANCODE_LANG4',
		'SDL_SCANCODE_LANG5',
		'SDL_SCANCODE_LANG6',
		'SDL_SCANCODE_LANG7',
		'SDL_SCANCODE_LANG8',
		'SDL_SCANCODE_LANG9',
		// 'SDL_SCANCODE_LOCKINGCAPSLOCK',
		// 'SDL_SCANCODE_LOCKINGNUMLOCK',
		// 'SDL_SCANCODE_LOCKINGSCROLLLOCK',
		'SDL_SCANCODE_NONUSBACKSLASH',
		'SDL_SCANCODE_NONUSHASH',
		'SDLK_AMPERSAND',
		'SDLK_ASTERISK',
		'SDLK_AT',
		'SDLK_CARET',
		'SDLK_COLON',
		'SDLK_DOLLAR',
		'SDLK_EXCLAIM',
		'SDLK_GREATER',
		'SDLK_HASH',
		'SDLK_LEFTPAREN',
		'SDLK_LESS',
		'SDLK_PERCENT',
		'SDLK_PLUS',
		'SDLK_QUESTION',
		'SDLK_QUOTEDBL',
		'SDLK_RIGHTPAREN',
		'SDLK_UNDERSCORE',
		'SDL_LOG_CATEGORY_APPLICATION',
		'SDL_LOG_CATEGORY_ERROR',
		'SDL_LOG_CATEGORY_SYSTEM',
		'SDL_LOG_CATEGORY_AUDIO',
		'SDL_LOG_CATEGORY_VIDEO',
		'SDL_LOG_CATEGORY_RENDER',
		'SDL_LOG_CATEGORY_INPUT',
		// 'SDL_LOG_CATEGORY_RESERVED',
		'SDL_LOG_CATEGORY_CUSTOM',
		'SDL_LOG_PRIORITY_VERBOSE',
		'SDL_LOG_PRIORITY_DEBUG',
		'SDL_LOG_PRIORITY_INFO',
		'SDL_LOG_PRIORITY_WARN',
		'SDL_LOG_PRIORITY_ERROR',
		'SDL_LOG_PRIORITY_CRITICAL',
		'SDL_NUM_LOG_PRIORITIES',
		'SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT',
		'SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT',
		'SDL_MESSAGEBOX_COLOR_BACKGROUND',
		'SDL_MESSAGEBOX_COLOR_TEXT',
		'SDL_MESSAGEBOX_COLOR_BUTTON_BORDER',
		'SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND',
		'SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED',
		'SDL_MESSAGEBOX_COLOR_MAX',
		'SDL_MESSAGEBOX_ERROR',
		'SDL_MESSAGEBOX_WARNING',
		'SDL_MESSAGEBOX_INFORMATION',
		'SDL_PIXELFORMAT_UNKNOWN',
		'SDL_PIXELFORMAT_INDEX1LSB',
		'SDL_PIXELFORMAT_INDEX1MSB',
		'SDL_PIXELFORMAT_INDEX4LSB',
		'SDL_PIXELFORMAT_INDEX4MSB',
		'SDL_PIXELFORMAT_INDEX8',
		'SDL_PIXELFORMAT_RGB332',
		'SDL_PIXELFORMAT_RGB444',
		'SDL_PIXELFORMAT_RGB555',
		'SDL_PIXELFORMAT_BGR555',
		'SDL_PIXELFORMAT_ARGB4444',
		'SDL_PIXELFORMAT_RGBA4444',
		'SDL_PIXELFORMAT_ABGR4444',
		'SDL_PIXELFORMAT_BGRA4444',
		'SDL_PIXELFORMAT_ARGB1555',
		'SDL_PIXELFORMAT_RGBA5551',
		'SDL_PIXELFORMAT_ABGR1555',
		'SDL_PIXELFORMAT_BGRA5551',
		'SDL_PIXELFORMAT_RGB565',
		'SDL_PIXELFORMAT_BGR565',
		'SDL_PIXELFORMAT_RGB24',
		'SDL_PIXELFORMAT_BGR24',
		'SDL_PIXELFORMAT_RGB888',
		'SDL_PIXELFORMAT_RGBX8888',
		'SDL_PIXELFORMAT_BGR888',
		'SDL_PIXELFORMAT_BGRX8888',
		'SDL_PIXELFORMAT_ARGB8888',
		'SDL_PIXELFORMAT_RGBA8888',
		'SDL_PIXELFORMAT_ABGR8888',
		'SDL_PIXELFORMAT_BGRA8888',
		'SDL_PIXELFORMAT_ARGB2101010',
		// 'SDL_PIXELFORMAT_RGBA32',
		// 'SDL_PIXELFORMAT_ARGB32',
		// 'SDL_PIXELFORMAT_BGRA32',
		// 'SDL_PIXELFORMAT_ABGR32',
		'SDL_PIXELFORMAT_YV12',
		'SDL_PIXELFORMAT_IYUV',
		'SDL_PIXELFORMAT_YUY2',
		'SDL_PIXELFORMAT_UYVY',
		'SDL_PIXELFORMAT_YVYU',
		'SDL_PIXELFORMAT_NV12',
		'SDL_PIXELFORMAT_NV21',
		'SDL_PIXELTYPE_UNKNOWN',
		'SDL_PIXELTYPE_INDEX1',
		'SDL_PIXELTYPE_INDEX4',
		'SDL_PIXELTYPE_INDEX8',
		'SDL_PIXELTYPE_PACKED8',
		'SDL_PIXELTYPE_PACKED16',
		'SDL_PIXELTYPE_PACKED32',
		'SDL_PIXELTYPE_ARRAYU8',
		'SDL_PIXELTYPE_ARRAYU16',
		'SDL_PIXELTYPE_ARRAYU32',
		'SDL_PIXELTYPE_ARRAYF16',
		'SDL_PIXELTYPE_ARRAYF32',
		'SDL_BITMAPORDER_NONE',
		'SDL_BITMAPORDER_4321',
		'SDL_BITMAPORDER_1234',
		'SDL_PACKEDORDER_NONE',
		'SDL_PACKEDORDER_XRGB',
		'SDL_PACKEDORDER_RGBX',
		'SDL_PACKEDORDER_ARGB',
		'SDL_PACKEDORDER_RGBA',
		'SDL_PACKEDORDER_XBGR',
		'SDL_PACKEDORDER_BGRX',
		'SDL_PACKEDORDER_ABGR',
		'SDL_PACKEDORDER_BGRA',
		'SDL_ARRAYORDER_NONE',
		'SDL_ARRAYORDER_RGB',
		'SDL_ARRAYORDER_RGBA',
		'SDL_ARRAYORDER_ARGB',
		'SDL_ARRAYORDER_BGR',
		'SDL_ARRAYORDER_BGRA',
		'SDL_ARRAYORDER_ABGR',
		'SDL_PACKEDLAYOUT_NONE',
		'SDL_PACKEDLAYOUT_332',
		'SDL_PACKEDLAYOUT_4444',
		'SDL_PACKEDLAYOUT_1555',
		'SDL_PACKEDLAYOUT_5551',
		'SDL_PACKEDLAYOUT_565',
		'SDL_PACKEDLAYOUT_8888',
		'SDL_PACKEDLAYOUT_2101010',
		'SDL_PACKEDLAYOUT_1010102',
		'SDL_POWERSTATE_UNKNOWN',
		'SDL_POWERSTATE_ON_BATTERY',
		'SDL_POWERSTATE_NO_BATTERY',
		'SDL_POWERSTATE_CHARGING',
		'SDL_POWERSTATE_CHARGED',
		'SDL_RENDERER_SOFTWARE',
		'SDL_RENDERER_ACCELERATED',
		'SDL_RENDERER_PRESENTVSYNC',
		'SDL_RENDERER_TARGETTEXTURE',
		'SDL_FLIP_NONE',
		'SDL_FLIP_HORIZONTAL',
		'SDL_FLIP_VERTICAL',
		// 'SDL_SYSWM_UNKNOWN',
		// 'SDL_SYSWM_WINDOWS',
		// 'SDL_SYSWM_X11',
		// 'SDL_SYSWM_DIRECTFB',
		// 'SDL_SYSWM_COCOA',
		// 'SDL_SYSWM_UIKIT',
		// 'SDL_SYSWM_WAYLAND',
		// 'SDL_SYSWM_MIR',
		// 'SDL_SYSWM_WINRT',
		// 'SDL_SYSWM_ANDROID',
		// 'SDL_SYSWM_VIVANTE',
		'SDL_TEXTUREACCESS_STATIC',
		'SDL_TEXTUREACCESS_STREAMING',
		'SDL_TEXTUREACCESS_TARGET',
		'SDL_TEXTUREMODULATE_NONE',
		'SDL_TEXTUREMODULATE_COLOR',
		'SDL_TEXTUREMODULATE_ALPHA',
		'SDL_THREAD_PRIORITY_LOW',
		'SDL_THREAD_PRIORITY_NORMAL',
		'SDL_THREAD_PRIORITY_HIGH',
		// 'SDL_WINRT_PATH_INSTALLED_LOCATION',
		// 'SDL_WINRT_PATH_LOCAL_FOLDER',
		// 'SDL_WINRT_PATH_ROAMING_FOLDER',
		// 'SDL_WINRT_PATH_TEMP_FOLDER',
		'SDL_WINDOWEVENT_NONE',
		'SDL_WINDOWEVENT_SHOWN',
		'SDL_WINDOWEVENT_HIDDEN',
		'SDL_WINDOWEVENT_EXPOSED',
		'SDL_WINDOWEVENT_MOVED',
		'SDL_WINDOWEVENT_RESIZED',
		'SDL_WINDOWEVENT_SIZE_CHANGED',
		'SDL_WINDOWEVENT_MINIMIZED',
		'SDL_WINDOWEVENT_MAXIMIZED',
		'SDL_WINDOWEVENT_RESTORED',
		'SDL_WINDOWEVENT_ENTER',
		'SDL_WINDOWEVENT_LEAVE',
		'SDL_WINDOWEVENT_FOCUS_GAINED',
		'SDL_WINDOWEVENT_FOCUS_LOST',
		'SDL_WINDOWEVENT_CLOSE',
		// 'SDL_WINDOWEVENT_TAKE_FOCUS',
		// 'SDL_WINDOWEVENT_HIT_TEST',
		'SDL_WINDOW_FULLSCREEN',
		'SDL_WINDOW_FULLSCREEN_DESKTOP',
		'SDL_WINDOW_OPENGL',
		'SDL_WINDOW_SHOWN',
		'SDL_WINDOW_HIDDEN',
		'SDL_WINDOW_BORDERLESS',
		'SDL_WINDOW_RESIZABLE',
		'SDL_WINDOW_MINIMIZED',
		'SDL_WINDOW_MAXIMIZED',
		'SDL_WINDOW_INPUT_GRABBED',
		'SDL_WINDOW_INPUT_FOCUS',
		'SDL_WINDOW_MOUSE_FOCUS',
		'SDL_WINDOW_FOREIGN',
		'SDL_WINDOW_ALLOW_HIGHDPI',
		'SDL_WINDOW_MOUSE_CAPTURE',
		// 'SDL_WINDOW_ALWAYS_ON_TOP',
		// 'SDL_WINDOW_SKIP_TASKBAR',
		// 'SDL_WINDOW_UTILITY',
		// 'SDL_WINDOW_TOOLTIP',
		// 'SDL_WINDOW_POPUP_MENU',
		'SDL_FALSE',
		'SDL_TRUE'
	],
	filter: (n) => {
		if (n.match(/^SDLK_/)) {
			return n.replace('SDLK_', 'KEY_');
		} else {
			return n.replace(/^SDL_/, '');
		}
	},
	template: (n) => "v8::Uint32::New(isolate, " + n + ")"
});

S.write('}\n\n');
S.write('}\n');

function writeConstants(spec) {
	spec.values.forEach((n) => {
		const name = spec.filter(n);
		const val = spec.template(n);
		S.write(`    ((void)exports->DefineOwnProperty(ctx, v8::String::NewFromUtf8(isolate, "${name}"), ${val}, attrs));\n`);
	});
}
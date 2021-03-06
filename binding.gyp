{
	"targets": [
		{
			"target_name": "sdl",
			"sources": [
				"src/constants.cc",
				"src/helpers.cc",
				"src/sdl.cc",
				"src/symbols.cc",

				"src/functions/audio.cc",
				"src/functions/2d_accelerated_rendering.cc",
				"src/functions/clipboard.cc",
				"src/functions/core.cc",
				"src/functions/cpu_feature_detection.cc",
				"src/functions/display_window.cc",
				"src/functions/errors.cc",
				"src/functions/events.cc",
				"src/functions/ext_image.cc",
				"src/functions/game_controller.cc",
				"src/functions/gl.cc",
				"src/functions/joystick.cc",
				"src/functions/keyboard.cc",
				"src/functions/mouse.cc",
				"src/functions/pixel_format_conversion.cc",
				"src/functions/surface_drawing.cc",
				"src/functions/timers.cc",

				"src/objects/audio_device.cc",
				"src/objects/cursor.cc",
				"src/objects/game_controller.cc",
				"src/objects/gl_context.cc",
				"src/objects/joystick.cc",
				"src/objects/renderer.cc",
				"src/objects/surface.cc",
				"src/objects/texture.cc",
				"src/objects/window.cc"
			],
			"include_dirs": [
				"include",
				"/usr/local/include"
			],
			"link_settings": {
				"libraries": [
					"-lSDL2",
					"-lSDL2_image",
					"-L/usr/local/lib",
					"-Wl,-rpath,/usr/local/lib"
				]
			}
		}
	]
}
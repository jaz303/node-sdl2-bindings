{
	"targets": [
		{
			"target_name": "sdl",
			"sources": [
				"src/sdl.cc",
				"src/constants.cc"
			],
			"include_dirs": [
				"src"
			],
			"link_settings": {
				"libraries": [
					"-lSDL2",
					"-lSDL2_gfx",
					"-lSDL2_mixer",
					"-lSDL2_image",
					"-L/usr/local/lib",
					"-Wl,-rpath,/usr/local/lib"
				]
			}
		}
	]
}
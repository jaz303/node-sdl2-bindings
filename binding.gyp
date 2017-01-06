{
	"targets": [
		{
			"target_name": "sdl",
			"sources": [
				"src/sdl.cc",
				"src/constants.cc"
			],
			"include_dirs": [
				"src",
				"/usr/local/include"
			],
			"link_settings": {
				"libraries": [
					"-lSDL2",
					"-lSDL2_gfx",
					"-lSDL2_image",
					"-lcairo",
					"-L/usr/local/lib",
					"-Wl,-rpath,/usr/local/lib"
				]
			}
		}
	]
}
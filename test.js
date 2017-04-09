const sdl = require('./');

console.log(sdl.INIT_EVERYTHING);

sdl.init(sdl.INIT_EVERYTHING);

const w = sdl.createWindow(
	"Test Window",
	sdl.WINDOWPOS_CENTERED,
	sdl.WINDOWPOS_CENTERED,
	800,
	600,
	sdl.WINDOW_RESIZABLE
);

console.log("window", w, sdl.getWindowId(w));

const surface = sdl.imageLoad("/Users/jason/Desktop/ml.jpg");
console.log(surface, surface.format);

while (!sdl.quitRequested()) {
	// sdl.pollEvent({})
}


// const data = new Uint32Array(surface.data);
// console.log(data.length);

// const window = sdl.createWindow("Test Window", 10, 10, 800, 600, 0);

// const surface = window.surface;
// const ctx = surface.getContext();

// console.log(ctx);

// sdl.showSimpleMessageBox("Hello", "This is a message from SDL");

// setTimeout(_ => {}, 5000);
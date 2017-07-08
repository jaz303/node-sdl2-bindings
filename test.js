const sdl = require('./');

console.log(sdl.INIT_EVERYTHING);

sdl.init(sdl.INIT_EVERYTHING);

// const w = sdl.createWindow(
// 	"Test Window",
// 	sdl.WINDOWPOS_CENTERED,
// 	sdl.WINDOWPOS_CENTERED,
// 	800,
// 	600,
// 	sdl.WINDOW_RESIZABLE
// );

// console.log("window", w, sdl.getWindowId(w));

// const surface = sdl.imageLoad("/Users/jason/Desktop/ml.jpg");
// console.log(surface, surface.format);

let adid = sdl.openAudioDevice((buffer) => {
	//console.log("callback fired!", buffer, buffer.length);
	for (let ix = 0; ix < buffer.length; ++ix) {
		buffer[ix] = Math.sin(ix * Math.PI * 2 / 44100 * 440) - 0.5;
	}
});

sdl.pauseAudioDevice(adid, false);

setInterval(() => {

}, 100);

// while (!sdl.quitRequested()) {
// 	// sdl.pollEvent({})
// }





// const data = new Uint32Array(surface.data);
// console.log(data.length);

// const window = sdl.createWindow("Test Window", 10, 10, 800, 600, 0);

// const surface = window.surface;
// const ctx = surface.getContext();

// console.log(ctx);

// sdl.showSimpleMessageBox("Hello", "This is a message from SDL");

// setTimeout(_ => {}, 5000);
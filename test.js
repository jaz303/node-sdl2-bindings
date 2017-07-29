const sdl = require('./');

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

// console.log("AUDIO_S8 = " + sdl.AUDIO_S8);

let t = 0;

const obtained = {};
let adid = sdl.openAudioDevice(
	sdl.getAudioDeviceName(0, 0),
	false,
	{
		freq: 44100,
		format: sdl.AUDIO_F32,
		channels: 2,
		samples: 0,
		callback(buffer) {
			for (let ix = 0; ix < buffer.length; ++ix) {
				buffer[ix] = Math.sin(t++ * Math.PI * 2 / 44100 * 220) - 0.5;
			}
		}
	},
	obtained,
	0
);

console.log(obtained);
console.log(obtained.format === sdl.AUDIO_F32);

sdl.pauseAudioDevice(adid, false);

setInterval(() => {
	for (let ix = 0; ix < 1000; ++ix) {
		let foo = new Uint32Array(1000);
	}
	//adid.destroy();
}, 10);

setTimeout(() => {
	sdl.closeAudioDevice(adid);
}, 2000);

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
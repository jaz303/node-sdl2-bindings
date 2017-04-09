const sdl = require('./build/Release/sdl');
module.exports = sdl;

// const DEFAULT_RMASK = 0xff000000;
// const DEFAULT_GMASK = 0x00ff0000;
// const DEFAULT_BMASK = 0x0000ff00;
// const DEFAULT_AMASK = 0x000000ff;

// sdl.createRGBSurface = function(width, height, depth, rMask, gMask, bMask, aMask) {
// 	if (typeof width !== 'number') throw new Error("surface width must be a number");
// 	if (typeof height !== 'number') throw new Error("surface height must be a number");
// 	if (arguments.length === 2) {
// 		return N.createRGBSurface(
// 			width, height, 32,
// 			DEFAULT_RMASK,
// 			DEFAULT_GMASK,
// 			DEFAULT_BMASK,
// 			DEFAULT_AMASK
// 		);
// 	} else if (arguments.length === 3) {

// 	}
// }

// sdl.createRGBSurfaceFromImage = function(imageData) {

// }

// sdl.createRGBSurfaceFromData = function(data, width, height, depth, pitch, rMask, gMask, bMask, aMask) {

// }
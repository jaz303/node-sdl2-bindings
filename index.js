const sdl = require('./build/Release/sdl');

const _joystickOpen = sdl.joystickOpen;
const _joystickClose = sdl.	joystickClose;

const joystickMap = {};

sdl.joystickOpen = function(index) {
	const js = _joystickOpen(index);
	joystickMap[sdl.joystickInstanceID(js)] = js;
	return js;
}

sdl.joystickClose = function(js) {
	const instanceId = sdl.joystickInstanceID(js);
	delete joystickMap[instanceId];
	_joystickClose(js);
}

sdl.joystickFromInstanceID = function(instanceId) {
	const js = joystickMap[instanceId];
	if (!js) {
		throw new Error("unknown joystick ID: " + instanceId);
	}
	return js;
}

module.exports = sdl;
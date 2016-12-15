## Notes

### Joysticks

To avoid maintaining an internal map of joystick instance ID to `SDL_Joystick*`, `joystickOpen(index)` currently returns a joystick instance ID rather than a wrapped pointer. This may change in future versions, and as such, the value returned from `joystickOpen()` should be treated as opaque.

### SDL_Image

Only implemented functions are `IMG_Init()`, `IMG_Quit()` and `IMG_Load()`.

## Things that won't be implemented
  
  - Logging - use existing node.js logging libraries
  - Threads - not applicable to node
  - Timers - use `setTimeout()`, `setInterval()`
  - File Abstraction - use `fs` package, streams etc.
  - Rectangle Functions - implement your own rectangle class or import one of the many on npm. Use objects with keys `x`, `y`, `width` and `height.
  - Shared Object Support

## Notes

### Joysticks

To avoid maintaining an internal map of joystick instance ID to `SDL_Joystick*`, `joystickOpen(index)` currently returns a joystick instance ID rather than a wrapped pointer. This may change in future versions, and as such, the value returned from `joystickOpen()` should be treated as opaque.

### SDL_Image

Only implemented functions are `IMG_Init()`, `IMG_Quit()` and `IMG_Load()`.

## Things that won't be implemented
  
  - Threads
  - Timers
  - File Abstraction
  - Rectangle Functions
  - Shared Object Support

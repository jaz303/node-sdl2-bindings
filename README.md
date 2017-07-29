# sdl2-bindings

SDL2 bindings for node.js v7+.

The bindings implemented by this library are pretty "raw"; little-to-no argument juggling is performed, and there are no optional arguments. I will document this more later.

## Exceptions

  - `createRGBSurface`, `createRGBSurfaceWithFormat`: no `flags` argument as it's obsolete. First argument should be `width`.

## Things that are still to be implemented

  - Hints
  - Game controller
  - 2D accelerated rendering
  - Pixel formats
  - Finish off surface handling
  - Figure out strategy for audio

## Functions not yet implemented

### Audio

  - `SDL_BuildAudioCVT`
  - `SDL_ConvertAudio`
  - `SDL_MixAudio`
  - `SDL_MixAudioFormat`

## Functions that won't be implemented

### Audio

  - `SDL_CloseAudio` (deprecated)
  - `SDL_OpenAudio` (deprecated)
  - `SDL_FreeWAV`
  - `SDL_LoadWAV` (use an npm module)
  - `SDL_LoadWAV_RW` (use an npm module)
  - `SDL_GetAudioStatus` (deprecated)
  - `SDL_LockAudio` (deprecated)

## Things that won't be implemented
  
  - Logging - use existing node.js logging libraries
  - Threads - not applicable to node
  - Timers - use `setTimeout()`, `setInterval()`
  - File Abstraction - use `fs` package, streams etc.
  - Rectangle Functions - implement your own rectangle class or import one of the many on npm. Use objects with keys `x`, `y`, `width` and `height`.
  - Shared Object Support

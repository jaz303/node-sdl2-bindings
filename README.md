# sdl2-bindings

SDL2 bindings for node.js v7+.

The bindings implemented by this library are pretty "raw"; little-to-no argument juggling is performed, and there are no optional arguments. I will document this more later.

## Things that are still to be implemented

  - Hints
  - Game controller
  - 2D accelerated rendering
  - Pixel formats
  - Finish off surface handling
  - Figure out strategy for audio

## Things that won't be implemented
  
  - Logging - use existing node.js logging libraries
  - Threads - not applicable to node
  - Timers - use `setTimeout()`, `setInterval()`
  - File Abstraction - use `fs` package, streams etc.
  - Rectangle Functions - implement your own rectangle class or import one of the many on npm. Use objects with keys `x`, `y`, `width` and `height`.
  - Shared Object Support

EVENT_TYPES = [
  {
    :cases => [:WINDOWEVENT],
    :key => :window,
    :args => {
      :timestamp => [:number, "window.timestamp"],
      :windowId => [:number, "window.windowID"],
      :event => [:number, "window.event"],
      :data1 => [:number, "window.data1"],
      :data2 => [:number, "window.data2"]
    }  
  }
]
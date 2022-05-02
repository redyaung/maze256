#ifndef Controls_hpp
#define Controls_hpp

#include <FastLED.h>
#include <LedControl.h>

#include "Macros.hpp"
#include "Renderer.hpp"
#include "Joystick.hpp"

namespace Controls {
  extern CRGB leds[VISION_NUM_LEDS];
  extern LedControl compassDisplay;
  extern Renderer renderer;
  extern Joystick joystick;
};

#endif

#include "Controls.hpp"

namespace Controls {
  CRGB leds[VISION_NUM_LEDS];
  LedControl compassDisplay(COMPASS_DIN, COMPASS_CLK, COMPASS_CS, 1);
  Renderer renderer(compassDisplay);
  Joystick joystick(JOYSTICK_VRX, JOYSTICK_VRY, JOYSTICK_SW);
};

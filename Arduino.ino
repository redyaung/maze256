#include <LedControl.h>
#include <FastLED.h>

#include "Comms.hpp"
#include "Compass.hpp"
#include "Direction.hpp"
#include "Joystick.hpp"
#include "Macros.hpp"
#include "Processor.hpp"
#include "Renderer.hpp"
#include "State.hpp"

// Global Variables
CRGB leds[VISION_NUM_LEDS];
LedControl compassDisplay(COMPASS_DIN, COMPASS_CLK, COMPASS_CS, 1);
Renderer renderer(compassDisplay);
Joystick joystick(JOYSTICK_VRX, JOYSTICK_VRY, JOYSTICK_SW);

// Function Declarations
static void listen();

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, VISION_DATA_PIN>(leds, VISION_NUM_LEDS);
  FastLED.setBrightness(8);                            // [0, 255]
  compassDisplay.shutdown(COMPASS_LED_INDEX, false);
  compassDisplay.setIntensity(COMPASS_LED_INDEX, 2);    // [0, 15]
  compassDisplay.clearDisplay(COMPASS_LED_INDEX);
}

void loop() {
  listen();

  static unsigned long joystickTimeout = 0;
  if (millis() > joystickTimeout) {
    Direction move = joystick.readDirection();
    if (move != Direction::None) {
      Comms::sendMove(move);
      joystickTimeout = millis() + 1000;
    }
  }

  static unsigned long renderTimeout = 0;
  if (millis() > renderTimeout) {
    renderer.renderWideVision(State::wideVision);
    renderer.renderCompass(State::compassDegree);
    FastLED.show();
    renderTimeout = millis() + 200;
  }
}

static void listen() {
  static byte content[64];
  static int contentSz = 0;
  if (Serial.available() > 0) {
    Comms::Type type = Comms::process(content, contentSz);
    if (type != Comms::Type::Invalid)
      Processor::process(type, content, contentSz);
  }
}

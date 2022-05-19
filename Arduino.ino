#include <LedControl.h>
#include <FastLED.h>

#include "Comms.hpp"
#include "Compass.hpp"
#include "Controls.hpp"
#include "Direction.hpp"
#include "Joystick.hpp"
#include "Macros.hpp"
#include "Processor.hpp"
#include "Renderer.hpp"
#include "State.hpp"

// Function Declarations
static void listen();

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, VISION_DATA_PIN>(Controls::leds, VISION_NUM_LEDS);
  FastLED.setBrightness(16);                                        // [0, 255]
  Controls::compassDisplay.shutdown(COMPASS_LED_INDEX, false);
  Controls::compassDisplay.setIntensity(COMPASS_LED_INDEX, 4);      // [0, 15]
  Controls::compassDisplay.clearDisplay(COMPASS_LED_INDEX);

  Controls::driver.begin();
  Controls::driver.setOscillatorFrequency(25000000);
  Controls::driver.setPWMFreq(50);

  for (int pin = 3; pin <= 10; pin++)
    Controls::remnants[pin - 3].attach(pin);

  // Lower all the walls
  for (int wall = 1; wall <= 24; wall++)
    Renderer::renderWall(wall, false);
}

void loop() {
  listen();

  static unsigned long joystickTimeout = 0;
  if (millis() > joystickTimeout) {
    Direction move = Controls::joystick.readDirection();
    if (move != Direction::None) {
      Comms::sendMove(move);
      joystickTimeout = millis() + 1000;
    }
  }

  static unsigned long renderTimeout = 0;
  if (millis() > renderTimeout) {
    // Renderer::renderWideVision(State::wideVision);
    // Renderer::renderCompass(State::compassDegree);
    // FastLED.show();
    renderTimeout = millis() + 200;
  }

  if (State::visionChanged) {
    Renderer::renderNormalVision(State::normalVision);
    State::visionChanged = false;
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

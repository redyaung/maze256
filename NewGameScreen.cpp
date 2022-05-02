#include "NewGameScreen.hpp"
#include "Screen.hpp"
#include "Macros.hpp"

NewGameScreen NewGameScreen::instance;

static const unsigned long joystickButtonDebounce = 50;

Screen& NewGameScreen::loop() {
  // Update internal state based on whether the user clicks the joystick
  static int lastState = HIGH;
  static int lastFlickerTime = 0;
  int state = digitalRead(JOYSTICK_SW);
  if (state != lastState) {
    lastFlickerTime = millis();
  } else if (millis() > lastFlickerTime + joystickButtonDebounce) {
    if (lastState == HIGH && state == LOW) {      // Button Pressed
      // TODO: Transition to a new screen
    }
  }
  lastState = state;
  // Display the screen on the 16x2 display periodically
}

NewGameScreen& NewGameScreen::make() {
  return instance;
}


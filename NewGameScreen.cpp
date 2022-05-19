#include "NewGameScreen.hpp"
#include "Screen.hpp"
#include "Controls.hpp"
#include "Macros.hpp"

static const unsigned long joystickButtonDebounce = 50;

NewGameScreen::NewGameScreen(bool rendered)
  : rendered(rendered) {}

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
  if (!rendered) {
    Controls::textDisplay.setCursor(1, 0);
    Controls::textDisplay.write("The Labyrinth");
    Controls::textDisplay.setCursor(4, 1);
    Controls::textDisplay.write("New Game");
    rendered = true;
  }
  return *this;
}

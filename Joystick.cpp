#include "Joystick.hpp"

#ifdef ARDUINO
  #include <Arduino.h>
#endif

static const int lower_bound = 256;     // 512 - 256
static const int higher_bound = 768;    // 512 + 256

Joystick::Joystick(int xPin, int yPin, int switchPin)
  : xPin(xPin), yPin(yPin), switchPin(switchPin) {}

Direction Joystick::readDirection() const {
  int x, y;
  // Deliberate double reading to stabilize analog readings
  analogRead(xPin), x = analogRead(xPin);
  analogRead(yPin), y = analogRead(yPin);
  if (y < lower_bound && x >= lower_bound && x <= higher_bound)
    return Direction::Up;
  if (y > higher_bound && x >= lower_bound && x <= higher_bound)
    return Direction::Down;
  if (x < lower_bound && y >= lower_bound && y <= higher_bound)
    return Direction::Left;
  if (x > higher_bound && y >= lower_bound && y <= higher_bound)
    return Direction::Right;
  return Direction::None;
}

bool Joystick::isPressed() const {
  int status = digitalRead(switchPin);
  return status == LOW;
}

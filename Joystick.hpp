#ifndef Joystick_hpp
#define Joystick_hpp

#include "Direction.hpp"

class Joystick {
public:
  Joystick(int xPin, int yPin, int switchPin);

  Direction readDirection() const;
  bool isPressed() const;

private:
  int xPin, yPin, switchPin;
};

#endif

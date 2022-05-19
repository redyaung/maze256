#ifndef Controls_hpp
#define Controls_hpp

#include <FastLED.h>
#include <LedControl.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>

#include "Macros.hpp"
#include "Joystick.hpp"

namespace Controls {
  extern CRGB leds[VISION_NUM_LEDS];
  extern LedControl compassDisplay;
  extern Joystick joystick;
  extern LiquidCrystal_I2C textDisplay;

  extern Adafruit_PWMServoDriver driver;
  extern Servo remnants[8];
};

#endif

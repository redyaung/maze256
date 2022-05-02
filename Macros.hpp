#ifndef Macros_hpp
#define Macros_hpp

// #define NDEBUG          // Un-define this macro when testing

#ifndef NDEBUG
  #define __Debug(a) Serial.print(a)
  #define __Debugln(a) Serial.println(a)
#else
  #define __Debug(a) 0
  #define __Debugln(a) 0
#endif

#ifndef ARDUINO
  #include <cassert>
  #include <Arduino.h>
#else
  #include <assert.h>
#endif

// Wide Vision Display: 16x16 Matrix Display
#define VISION_WIDTH 16
#define VISION_HEIGHT 16
#define VISION_NUM_LEDS VISION_WIDTH * VISION_HEIGHT
#define VISION_DATA_PIN 12

// Compass Display: 8x8 LED Display
#define COMPASS_DIN 11
#define COMPASS_CLK 24
#define COMPASS_CS 22
#define COMPASS_LED_INDEX 0

// Joystick
#define JOYSTICK_VRX A0
#define JOYSTICK_VRY A1
#define JOYSTICK_SW 46

#endif

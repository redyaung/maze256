#include "Controls.hpp"

namespace Controls {
  CRGB leds[VISION_NUM_LEDS];
  LedControl compassDisplay(COMPASS_DIN, COMPASS_CLK, COMPASS_CS, 1);
  Joystick joystick(JOYSTICK_VRX, JOYSTICK_VRY, JOYSTICK_SW);
  LiquidCrystal_I2C textDisplay(TEXT_DISPLAY_LCD_ADDRESS, TEXT_DISPLAY_COLS, TEXT_DISPLAY_ROWS);

  Adafruit_PWMServoDriver driver;
  Servo remnants[8];
};

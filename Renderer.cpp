#include "Compass.hpp"
#include "Controls.hpp"
#include "Renderer.hpp"
#include "Vision.hpp"
#include "Macros.hpp"

#ifdef ARDUINO
  #include <FastLED.h>
  #include <LEDControl.h>
#endif

namespace Renderer {
  static void setVisionPixel(int row, int col, CRGB color);

  static const int horizontalWallMapping[4][3] = {
    {18, 17, 16},
    {5, 8, 3},
    {6, 11, 4},
    {22, 23, 24}
  }, verticalWallMapping[3][4] = {
    {19, 9, 7, 15},
    {20, 1, 2, 14},
    {21, 12, 10, 13}
  };
  static const int driverServoMin = 150;
  static const int driverServoMax = 600;
  static const int servoDelay = 5;

  void renderWall(int wallIndex, bool state) {
    state = !state;
    if (wallIndex >= 2 && wallIndex <= 17) {
      int pin = wallIndex >= 16 ? wallIndex - 16 : wallIndex;
      if (wallIndex == 8 || wallIndex == 12 || wallIndex == 11)
        state = !state;     // These turn counter clockwise
      int from = state ? driverServoMin : driverServoMax;
      int to = state ? driverServoMax : driverServoMin;
      int increment = state ? 1 : -1;
      for (int pulseWidth = from; pulseWidth != to; pulseWidth += increment) {
        Controls::driver.setPWM(pin, 0, pulseWidth);
        delay(servoDelay);
      }
    } else {
      int pin = wallIndex == 1 ? 3 : wallIndex - 14;
      int remnantIndex = pin - 3;
      int from = state ? 0 : 180;
      int to = state ? 180 : 0;
      int increment = state ? 1 : -1;
      for (int degree = from; degree != to; degree += increment) {
        Controls::remnants[remnantIndex].write(degree);
        delay(servoDelay);
      }
    }
  }

  void renderNormalVision(Vision<3> vision) {
    static Vision<3> previous;
    for (int row = 0; row < vision.range() + 1; row++) {
      for (int col = 0; col < vision.range(); col++) {
        bool state = vision.horizontals[row][col];
        if (previous.horizontals[row][col] != vision.horizontals[row][col])
          renderWall(horizontalWallMapping[row][col], state);
      }
    }
    for (int row = 0; row < vision.range(); row++) {
      for (int col = 0; col < vision.range() + 1; col++) {
        bool state = vision.verticals[row][col];
        if (previous.verticals[row][col] != vision.verticals[row][col])
          renderWall(verticalWallMapping[row][col], state);
      }
    }
    previous = vision;
  }

  void renderWideVision(Vision<5> vision) {
    static Vision<5> previous;
    static bool isInitialRender = true;
    static CRGB colorMap[2] = { CRGB::Black, CRGB::Navy };

    bool midActivated[6][6];
    for (int i = 0; i < 36; i++)
      midActivated[i / 6][i % 6] = false;

    // Horizontal Walls
    for (int row = 0; row < 6; row++) {
      for (int col = 0; col < 5; col++) {
        midActivated[row][col] = midActivated[row][col] || vision.horizontals[row][col];
        midActivated[row][col + 1] = midActivated[row][col + 1] || vision.horizontals[row][col];
        if (previous.horizontals[row][col] == vision.horizontals[row][col])
          continue;
        int pixelRow = row * 3, pixelCol = col * 3;
        CRGB color = colorMap[vision.horizontals[row][col]];
        setVisionPixel(pixelRow, pixelCol + 1, color);
        setVisionPixel(pixelRow, pixelCol + 2, color);
      }
    }
    // Vertical Walls (!!Code Duplication)
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 6; col++) {
        midActivated[row][col] = midActivated[row][col] || vision.verticals[row][col];
        midActivated[row + 1][col] = midActivated[row + 1][col] || vision.verticals[row][col];
        if (previous.verticals[row][col] == vision.verticals[row][col])
          continue;
        int pixelRow = row * 3, pixelCol = col * 3;
        CRGB color = colorMap[vision.verticals[row][col]];
        setVisionPixel(pixelRow + 1, pixelCol, color);
        setVisionPixel(pixelRow + 2, pixelCol, color);
      }
    }
    // Mid Pieces
    for (int row = 0; row < 6; row++) {
      for (int col = 0; col < 6; col++) {
        setVisionPixel(row * 3, col * 3, colorMap[midActivated[row][col]]);
      }
    }
    // Store the previous render
    previous = vision;
    // Player Render
    if (isInitialRender) {
      setVisionPixel(7, 7, CRGB::Red);
      setVisionPixel(7, 8, CRGB::Red);
      setVisionPixel(8, 7, CRGB::Red);
      setVisionPixel(8, 8, CRGB::Red);
      isInitialRender = false;
    }
  }

  void renderCompass(float degree) {
    static Compass::_Direction previous = Compass::_Direction::_90;
    Compass::_Direction d = Compass::calcDirection(degree);
    if (previous == d)
      return;
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        bool painted = Compass::isPaintedPixel(d, row, col);
        Controls::compassDisplay.setLed(0, row, col, painted);
      }
    }
    previous = d;
  }

  static void setVisionPixel(int row, int col, CRGB color) {
    int colBegin = col * VISION_HEIGHT;
    int offset = col % 2 == 0 ? row : VISION_HEIGHT - row - 1;
    int index = colBegin + offset;
    Controls::leds[index] = color;
  }
};

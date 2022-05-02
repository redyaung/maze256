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

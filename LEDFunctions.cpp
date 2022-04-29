#include <FastLED.h>

extern CRGB leds[NUM_LEDS];

void setPixel(int row, int col, CRGB color) {
 int colBegin = col * HEIGHT;
 int offset = col % 2 == 0 ? row : HEIGHT - row - 1;
 int index = colBegin + offset;
 leds[index] = color;
}

void setWideVision(Vision<5> vision) {
 // Render Horizontal Walls
 for (int row = 0; row < vision.horizontals.size(); row++) {
   for (int col = 0; col < vision.horizontals[row].size(); col++) {
     int pixelRow = row * 3, pixelCol = col * 3;
     CRGB color = vision.horizontals[row][col] ? CRGB::Navy : CRGB::White;
     for (int count = 0; count < 4; count++)
       setPixel(pixelRow, pixelCol + count, color);
   }
 }
 // Render Vertical Walls (Duplication)
 for (int row = 0; row < vision.verticals.size(); row++) {
   for (int col = 0; col < vision.verticals[row].size(); col++) {
     int pixelRow = row * 3, pixelCol = col * 3;
     CRGB color = vision.verticals[row][col] ? CRGB::Navy : CRGB::White;
     for (int count = 0; count < 4; count++)
       setPixel(pixelRow + count, pixelCol, color);
   }
 }
 // Fill the cells white
 for (int row = 0; row < 5; row++) {
   for (int col = 0; col < 5; col++) {
     int pixelRow = row * 3 + 1, pixelCol = col * 3 + 1;
     CRGB color = row == 2 && col == 2 ? CRGB::Red : CRGB::White;
     for (int rowOff = 0; rowOff < 2; rowOff++)
       for (int colOff = 0; colOff < 2; colOff++)
         setPixel(pixelRow + rowOff, pixelCol + colOff, color);
   }
 }
}

#include <LedControl.h>
#include <FastLED.h>

#include "State.hpp"
#include "Direction.hpp"
#include "Compass.hpp"

#define WIDTH 16
#define HEIGHT 16
#define NUM_LEDS WIDTH * HEIGHT
#define LED_DATA_PIN 12

#define COMPASS_DIN 11
#define COMPASS_CLK 24
#define COMPASS_CS 22
#define COMPASS_LED_INDEX 0

#define SERIAL_BUFFER_CAPACITY 64
#define TYPE_DELIMITER ':'
#define TERMINATOR ']'
#define INITIATOR '['

#ifndef ARDUINO
#include <cassert>
#else
#include <assert.h>
#endif

#define NDEBUG        // Use this when connecting to Processing

#ifndef NDEBUG
#define SERIAL_DEBUG(a) Serial.print(a)
#define SERIAL_DEBUGLN(a) Serial.println(a)
#else
#define SERIAL_DEBUG(a) 0
#define SERIAL_DEBUGLN(a) 0
#endif

enum class Difficulty {
 Very_Easy = 0, Easy = 1, Normal = 2, Hard = 3
};

///// Globals
CRGB leds[NUM_LEDS];
byte serialBuffer[SERIAL_BUFFER_CAPACITY];
int serialBufferSize;
State &state = State::getInstance();
LedControl compassDisplay(COMPASS_DIN, COMPASS_CLK, COMPASS_CS, 1);

template<typename Enum>
byte encode(Enum element) {
 return static_cast<int>(element);
}

template<typename Enum>
Enum decode(byte encoded) {
 return static_cast<Enum>(encoded);
}

void handleNormalVision(byte *content, int size) {
 assert(size == 24);
 int index = 0;
 for (int row = 0; row < 4; row++)
   for (int col = 0; col < 3; col++, index++)
     state.normalVision.horizontals[row][col] = content[index];
 for (int row = 0; row < 3; row++)
   for (int col = 0; col < 4; col++, index++)
     state.normalVision.verticals[row][col] = content[index];
 SERIAL_DEBUGLN("Setting normal vision.");
}

void handleWideVisionHorizontals(byte *content, int size) {
 assert(size == 30);
 int index = 0;
 for (int row = 0; row < 6; row++)
   for (int col = 0; col < 5; col++, index++)
     state.wideVision.horizontals[row][col] = content[index];
 SERIAL_DEBUGLN("Setting widevision horizontals.");
}

void handleWideVisionVerticals(byte *content, int size) {
 assert(size == 30);
 int index = 0;
 for (int row = 0; row < 5; row++)
   for (int col = 0; col < 6; col++, index++)
     state.wideVision.verticals[row][col] = content[index];
 SERIAL_DEBUGLN("Setting widevision verticals.");
}

void handleCompassDegree(byte *content, int size) {
 content[size] = 0;
 String floatStr = String((char *)content);
 state.compassDegree = floatStr.toFloat();
 SERIAL_DEBUG("Setting compass direction to ");
 SERIAL_DEBUGLN(String(state.compassDegree, 3));
}

void handleAutopilot(byte *content, int size) {

}

void handleIsGameOver(byte *content, int size) {
 assert(size == 1);
 state.isGameOver = content[0];
 SERIAL_DEBUG("Setting Game Over to ");
 SERIAL_DEBUGLN(state.isGameOver);
}

void handleSerialBuffer() {
 // serialBuffer contains "[<Type>:<Contents>" without the terminator.
 int delimIndex;
 for (
   delimIndex = 0;
   delimIndex < serialBufferSize && serialBuffer[delimIndex] != TYPE_DELIMITER;
   delimIndex++
 ) {}
 serialBuffer[delimIndex] = 0;
 String type = String((char *) serialBuffer);
 byte *content = serialBuffer + delimIndex + 1;

#ifndef NDEBUG
 SERIAL_DEBUGLN(type);
 serialBuffer[serialBufferSize] = 0;
 SERIAL_DEBUGLN(String((char *) content));
 for (int i = 0; content[i] != 0; i++) {
   SERIAL_DEBUG(int(content[i]));
   SERIAL_DEBUG(" ");
 }
 SERIAL_DEBUGLN();
#endif

 int size = serialBufferSize - delimIndex - 1;
 if (type == "NormalVision") {
   handleNormalVision(content, size);
 } else if (type == "WideVisionH") {
   handleWideVisionHorizontals(content, size);
 } else if (type == "WideVisionV") {
   handleWideVisionVerticals(content, size);
 } else if (type == "Compass") {
   handleCompassDegree(content, size);
 } else if (type == "Autopilot") {
   handleAutopilot(content, size);
 } else if (type == "IsGameOver") {
   handleIsGameOver(content, size);
 }
}

void sendPlayerMove(Direction d) {
 Serial.print("[Move:");
 Serial.write(encode(d));
 Serial.print("]");
}

void sendConfiguration(Difficulty diff) {
 Serial.print("[Config:");
 Serial.write(encode(diff));
 Serial.print("]");
}

void sendAutopilotRequest() {
 Serial.print("[AutopilotRequest:]");
}

void sendGiveUpRequest() {
 Serial.print("[GiveUpRequest:]");
}

void setPixel(int row, int col, CRGB color) {
 int colBegin = col * HEIGHT;
 int offset = col % 2 == 0 ? row : HEIGHT - row - 1;
 int index = colBegin + offset;
 leds[index] = color;
}

void setWideVision(Vision<5> vision) {
 static bool prevActivated[16][16];
 bool activated[16][16];
 for (int i = 0; i < 16; i++)
   for (int j = 0; j < 16; j++)
     activated[i][j] = false;
 // Render Horizontal Walls
 for (int row = 0; row < 6; row++) {
   for (int col = 0; col < 5; col++) {
     int pixelRow = row * 3, pixelCol = col * 3;
     CRGB color = vision.horizontals[row][col] ? CRGB::Navy : CRGB::Black;
     for (int count = 0; count < 4; count++) {
       activated[pixelRow][pixelCol + count] = vision.horizontals[row][col];
       if (activated[pixelRow][pixelCol + count] != prevActivated[pixelRow][pixelCol + count])
         setPixel(pixelRow, pixelCol + count, color);
       prevActivated[pixelRow][pixelCol + count] = activated[pixelRow][pixelCol + count];
     }
   }
 }
 // Render Vertical Walls (Duplication)
 for (int row = 0; row < 5; row++) {
   for (int col = 0; col < 6; col++) {
     int pixelRow = row * 3, pixelCol = col * 3;
     for (int count = 0; count < 4; count++) {
       activated[pixelRow + count][pixelCol] = activated[pixelRow + count][pixelCol] || vision.verticals[row][col];
       CRGB color = activated[pixelRow + count][pixelCol] ? CRGB::Navy : CRGB::Black;
       if (activated[pixelRow + count][pixelCol] != prevActivated[pixelRow + count][pixelCol])
         setPixel(pixelRow + count, pixelCol, color);
       prevActivated[pixelRow + count][pixelCol] = activated[pixelRow + count][pixelCol];
     }
   }
 }
 // Paint the player red
 for (int row = 7; row < 9; row++) {
   for (int col = 7; col < 9; col++) {
     setPixel(row, col, CRGB::Red);
   }
 }
}

void setCompass(float degree) {
  Compass::_Direction d = Compass::calcDirection(degree);
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      bool painted = Compass::isPaintedPixel(d, row, col);
      compassDisplay.setLed(0, row, col, painted);
    }
  }
}

void setup() {
 Serial.begin(115200);

 FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
 FastLED.setBrightness(8);           // [0, 255]
 FastLED.show();

 compassDisplay.shutdown(COMPASS_LED_INDEX, false);
 compassDisplay.setIntensity(COMPASS_LED_INDEX, 8);  // [0, 15]
 compassDisplay.clearDisplay(COMPASS_LED_INDEX);
}

size_t readBytesUntilTerminator(byte terminator, byte *buffer, size_t count, bool &hasTerminated) {
 int i = 0;
 while (i < count && Serial.available()) {
   byte c = Serial.read();
   if (c == terminator) {
     hasTerminated = true;
     return i;
   }
   buffer[i] = c;
   i++;
 }
 hasTerminated = false;
 return i;
}

bool initiatorRead = false;

void loop() {
  static unsigned long timeout = 0;

  if (Serial.available() > 0) {
    if (!initiatorRead) {
      initiatorRead = Serial.find(INITIATOR);
      serialBufferSize = 0;
      SERIAL_DEBUG("Initiator Read: ");
      SERIAL_DEBUGLN(initiatorRead);
    } else {
      bool terminatorFound = false;
      size_t newSize = readBytesUntilTerminator(TERMINATOR, serialBuffer + serialBufferSize, SERIAL_BUFFER_CAPACITY, terminatorFound);
      serialBufferSize += newSize;
      SERIAL_DEBUG("After Terminator: ");
      SERIAL_DEBUGLN((char *) serialBuffer);
      if (terminatorFound) {
        // We can be sure that the content is at least 1.
        serialBuffer[serialBufferSize] = 0;
        handleSerialBuffer();
      }
      initiatorRead = !terminatorFound;
    }
  } else {
    SERIAL_DEBUGLN("Data unavailable.");
  }
  
  if (millis() > timeout) {
    setWideVision(state.wideVision);
    setCompass(state.compassDegree);

    FastLED.show();
    timeout = millis() + 500;
  }
}

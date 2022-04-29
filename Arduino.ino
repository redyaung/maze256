#include <FastLED.h>
#include <assert.h>

#include "State.hpp"
#include "Direction.hpp"

#define WIDTH 16
#define HEIGHT 16
#define NUM_LEDS WIDTH * HEIGHT
#define LED_DATA_PIN 8

#define TYPE_DELIMITER ':'
#define TERMINATOR ']'
#define INITIATOR '['

enum class Difficulty {
  Very_Easy = 0, Easy = 1, Normal = 2, Hard = 3
};

CRGB leds[NUM_LEDS];
byte serialBuffer[SERIAL_BUFFER_CAPACITY];
int serialBufferSize;
State &state = State::getInstance();

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
  for (int row = 0; row < 3; row++)
    for (int col = 0; col < 4; col++, index++)
      state.normalVision.horizontals[row][col] = content[index];
  for (int row = 0; row < 4; row++)
    for (int col = 0; col < 3; col++, index++)
      state.normalVision.verticals[row][col] = content[index];
}

void handleWideVision(byte *content, int size) {
  assert(size == 24);
  int index = 0;
  for (int row = 0; row < 5; row++)
    for (int col = 0; col < 6; col++, index++)
      state.wideVision.horizontals[row][col] = content[index];
  for (int row = 0; row < 6; row++)
    for (int col = 0; col < 5; col++, index++)
      state.wideVision.verticals[row][col] = content[index];
}

void handleCompassDirection(byte *content, int size) {
  content[size] = 0;
  String floatStr = String(content);
  state.compassDirection = floatStr.toFloat();
}

void handleAutopilot(byte *content, int size) {

}

void handleIsGameOver(byte *content, int size) {
  assert(size == 1);
  state.isGameOver = content[0];
}

void handleSerialBuffer() {
  // serialBuffer contains "[<Type>:<Contents>" without the terminator.
  int delimIndex;
  for (
    delimIndex = 0;
    delimIndex < serialBufferSIZE && serialBuffer[delimIndex] != TYPE_DELIMITER;
    delimIndex++
  ) {}
  serialBuffer[delimIndex] = 0;
  String type = String(serialBuffer + 1);
  byte *content = serialBuffer + delimIndex + 1;
  int size = serialBufferSize - delimIndex - 1;
  if (type == "NormalVision") {
    handleNormalVision(content, size);
  } else if (type == "WideVision") {
    handleWideVision(content, size);
  } else if (type == "Compass") {
    handleCompassDirection(content, size);
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

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(32);
  FastLED.show();

  while (!Serial) {}
}

void loop() {
  if (Serial.available()) {
    serialBufferSize = Serial.readBytesUntil(TERMINATOR, serialBuffer, SERIAL_BUFFER_CAPACITY);
    if (serialBufferSize > 0)
      handleSerialBuffer();
  }
}

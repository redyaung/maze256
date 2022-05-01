#include "Processor.hpp"
#include "State.hpp"
#include "Macros.hpp"

using namespace Processor;

static State &state = State::getInstance();

void Processor::process(Comms::Type type, byte *content, int size) {
  switch (type) {
    case Comms::Type::NormalVision:
      processNormalVision(content, size);
      break;
    case Comms::Type::WideVisionH:
      processWideVisionHorizontals(content, size);
      break;
    case Comms::Type::WideVisionV:
      processWideVisionVerticals(content, size);
      break;
    case Comms::Type::Compass:
      processCompass(content, size);
      break;
    case Comms::Type::Autopilot:
      processAutopilot(content, size);
      break;
    case Comms::Type::IsGameOver:
      processIsGameOver(content, size);
      break;
  }
}

void Processor::processNormalVision(byte *content, int size) {
  assert(size == 24);
  int index = 0;
  for (int row = 0; row < 4; row++)
    for (int col = 0; col < 3; col++, index++)
      state.normalVision.horizontals[row][col] = content[index];
  for (int row = 0; row < 3; row++)
    for (int col = 0; col < 4; col++, index++)
      state.normalVision.verticals[row][col] = content[index];
  __Debugln("Setting normal vision.");
}

void Processor::processWideVisionHorizontals(byte *content, int size) {
  assert(size == 30);
  int index = 0;
  for (int row = 0; row < 6; row++)
    for (int col = 0; col < 5; col++, index++)
      state.wideVision.horizontals[row][col] = content[index];
  __Debugln("Setting widevision horizontals.");
}

void Processor::processWideVisionVerticals(byte *content, int size) {
  assert(size == 30);
  int index = 0;
  for (int row = 0; row < 5; row++)
    for (int col = 0; col < 6; col++, index++)
      state.wideVision.verticals[row][col] = content[index];
  __Debugln("Setting widevision verticals.");
}

void Processor::processCompass(byte *content, int size) {
  content[size] = '\0';
  String floatStr = String((char *)content);
  state.compassDegree = floatStr.toFloat();
  __Debug("Setting compass direction to ");
  __Debugln(String(state.compassDegree, 3));
}

void Processor::processAutopilot(byte *content, int size) {

}

void Processor::processIsGameOver(byte *content, int size) {
  assert(size == 1);
  state.isGameOver = content[0];
  __Debug("Setting Game Over to ");
  __Debugln(state.isGameOver);
}

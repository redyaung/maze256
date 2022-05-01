#ifndef Processor_hpp
#define Processor_hpp

#include "Comms.hpp"

namespace Processor {

  void process(Comms::Type type, byte *content, int size);

  void processNormalVision(byte *content, int size);
  void processWideVisionHorizontals(byte *content, int size);
  void processWideVisionVerticals(byte *content, int size);
  void processCompass(byte *content, int size);
  void processAutopilot(byte *content, int size);
  void processIsGameOver(byte *content, int size);
};

#endif

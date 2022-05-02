#ifndef Comms_hpp
#define Comms_hpp

#include "Direction.hpp"
#include "State.hpp"

namespace Comms {
  enum class Type {
    NormalVision,
    WideVisionH,
    WideVisionV,
    Compass,
    IsGameOver,
    Invalid
  };

  Type process(byte *content, int &size);
  void sendMove(Direction dir);
  void sendConfig(Difficulty diff);
  void sendAutopilotRequest();
  void sendGiveUpRequest();
};

#endif

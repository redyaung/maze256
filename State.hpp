#ifndef State_hpp
#define State_hpp

#include "Vision.hpp"

class State {
public:
  Vision<3> normalVision;
  Vision<5> wideVision;
  float compassDirection;
  bool isGameOver = false;

  static State& getInstance();

private:
  static State instance;
  State() = default;
  State(const State&) = delete;
  State& operator=(const State&) = delete;
};

#endif

#ifndef State_hpp
#define State_hpp

#include "Vision.hpp"

enum class Difficulty {
  Very_Easy = 0, Easy = 1, Normal = 2, Hard = 3
};

class State {
public:
  Vision<3> normalVision;
  Vision<5> wideVision;
  float compassDegree = 0.0;
  bool isGameOver = false;
  Difficulty difficulty = Difficulty::Normal;

  static State& getInstance();

private:
  static State instance;
  State() = default;
  State(const State&) = delete;
  State& operator=(const State&) = delete;
};

#endif

#include "State.hpp"

namespace State {
  Vision<3> normalVision;
  Vision<5> wideVision;
  bool visionChanged = false;

  float compassDegree = 0.0;
  bool isGameOver = false;
  Difficulty difficulty = Difficulty::Normal;
};

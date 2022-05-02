#ifndef State_hpp
#define State_hpp

#include "Vision.hpp"

enum class Difficulty {
  Very_Easy = 0, Easy = 1, Normal = 2, Hard = 3
};

namespace State {
  extern Vision<3> normalVision;
  extern Vision<5> wideVision;
  extern float compassDegree;
  extern bool isGameOver;
  extern Difficulty difficulty;
};

#endif

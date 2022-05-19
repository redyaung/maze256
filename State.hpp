#ifndef State_hpp
#define State_hpp

#include "Difficulty.hpp"
#include "Vision.hpp"

namespace State {
  extern Vision<3> normalVision;
  extern Vision<5> wideVision;
  extern bool visionChanged;

  extern float compassDegree;
  extern bool isGameOver;
  extern Difficulty difficulty;
};

#endif

#ifndef Renderer_hpp
#define Renderer_hpp

#include "Vision.hpp"

namespace Renderer {
  void renderNormalVision(Vision<3> vision);
  void renderWideVision(Vision<5> vision);
  void renderCompass(float degree);

  void renderWall(int wallIndex, bool state);
};

#endif

#ifndef Renderer_hpp
#define Renderer_hpp

#ifdef ARDUINO
#include <Arduino.h>
#include <FastLED.h>
#include <LedControl.h>
#endif

#include "Vision.hpp"

class Renderer {
public:
  Renderer(LedControl compassDisp);

  void renderWideVision(Vision<5> vision);
  void renderCompass(float degree);

private:
  LedControl compassDisp;
};

#endif

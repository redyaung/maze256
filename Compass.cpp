#include "Compass.hpp"

#ifndef ARDUINO
#include <cstdint>
#include <cmath>
#include <cassert>
using byte = uint8_t;
#else
#include <Arduino.h>
#include <math.h>
#include <assert.h>
#endif

using namespace Compass;

static byte pixels_0[8] = {
  0b00011000, 0b00001100, 0b00000110, 0b11111111, 0b11111111, 0b00000110, 0b00001100, 0b00011000
};
static byte pixels_22p5[8] = {
  0b00000000, 0b00011100, 0b00001110, 0b00000111, 0b00011111, 0b01110011, 0b11000011, 0b00000011
};
static byte pixels_45[8] = {
  0b11111111, 0b11111111, 0b00001111, 0b00011111, 0b00111011, 0b01110011, 0b11100011, 0b11000011
};

_Direction Compass::calcDirection(float degree) {
  float pi_16 = M_PI / 16.0;
  if (degree < -7 * pi_16) {
    return _Direction::_m90;
  } else if (degree < -5 * pi_16) {
    return _Direction::_m67p5;
  } else if (degree < -3 * pi_16) {
    return _Direction::_m45;
  } else if (degree < -pi_16) {
    return _Direction::_m22p5;
  } else if (degree < pi_16) {
    return _Direction::_0;
  } else if (degree < 3 * pi_16) {
    return _Direction::_22p5;
  } else if (degree < 5 * pi_16) {
    return _Direction::_45;
  } else if (degree < 7 * pi_16) {
    return _Direction::_67p5;
  } else {
    return _Direction::_90;
  }
}

static bool getBit(byte b, int bit) {
  return (b >> bit) & 0x1;
}

static _Direction complement(_Direction dir) {
  assert(static_cast<int>(dir) >= 0);
  return static_cast<_Direction>(90 - static_cast<int>(dir));
}

static _Direction negative(_Direction dir) {
  return static_cast<_Direction>(-static_cast<int>(dir));
}

bool Compass::isPaintedPixel(_Direction dir, int row, int col) {
  assert(row >= 0 && row < 8);
  assert(col >= 0 && col < 8);

  if (dir == _Direction::_0) {
    return getBit(pixels_0[row], 7 - col);
  } else if (dir == _Direction::_22p5) {
    return getBit(pixels_22p5[row], 7 - col);
  } else if (dir == _Direction::_45) {
    return getBit(pixels_45[row], 7 - col);
  } else if (static_cast<int>(dir) >= 0) {
    return isPaintedPixel(complement(dir), 7 - col, 7 - row);
  } else {      // Negative Angles
    return isPaintedPixel(negative(dir), 7 - row, col);
  }
}

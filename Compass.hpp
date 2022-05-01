#ifndef Compass_hpp
#define Compass_hpp

namespace Compass {
  enum class _Direction : int {
    _m90 = -90, _m67p5 = -67, _m45 = -45, _m22p5 = -23, _0 = 0,
    _22p5 = 23, _45 = 45, _67p5 = 67, _90 = 90
  };

  _Direction calcDirection(float degree);
  bool isPaintedPixel(_Direction dir, int row, int col);
};

#endif

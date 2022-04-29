#ifndef Vision_hpp
#define Vision_hpp

#ifndef ARDUINO
#include <cstddef>
#endif

#include "Vec.hpp"

template<size_t Range>
struct Vision {
  Vec<Vec<bool, Range>, Range + 1> horizontals;
  Vec<Vec<bool, Range + 1>, Range> verticals;

  Vision()
    : horizontals(Range + 1, Vec<bool, Range>(Range, false)),
      verticals(Range, Vec<bool, Range + 1>(Range + 1, false)) {}
};

#endif

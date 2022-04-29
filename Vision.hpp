#ifndef Vision_hpp
#define Vision_hpp

#ifndef ARDUINO
#include <cstddef>
#endif

template<size_t Range>
struct Vision {
  bool horizontals[Range][Range + 1];
  bool verticals[Range + 1][Range];

  Vision();

  size_t range() const;
};

template<size_t Range>
Vision<Range>::Vision() {
  for (int row = 0; row < Range; row++)
    for (int col = 0; col < Range + 1; col++)
      horizontals[row][col] = false;
  for (int row = 0; row < Range + 1; row++)
    for (int col = 0; col < Range; col++)
      verticals[row][col] = false;
}

template<size_t Range>
size_t Vision<Range>::range() const {
  return Range;
}

#endif

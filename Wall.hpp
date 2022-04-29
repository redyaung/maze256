#ifndef Wall_hpp
#define Wall_hpp

#include "Cell.hpp"
#include "Vec.hpp"

#ifndef ARDUINO
#include <cstddef>
#endif

class Wall {
public:
  bool exist;

  Wall();
  Wall(bool exist);
  Wall(Cell A, Cell B);

  Wall(const Wall &other);
  Wall& operator=(const Wall &other);

  Cell neighbor(Cell cell) const;

  template<size_t count>
  static Vec<Wall, count> existing(Vec<Wall, count> walls);

private:
  Cell A, B;
};

template<size_t count>
Vec<Wall, count> Wall::existing(Vec<Wall, count> walls) {
  Vec<Wall, count> existingWalls;
  for (size_t i = 0; i < walls.size(); i++)
    if (walls[i].exist)
      existingWalls.add(walls[i]);
  return existingWalls;
}

#endif

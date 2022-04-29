#ifndef Move_hpp
#define Move_hpp

#include "Cell.hpp"
#include "Direction.hpp"

class Move {
public:
  Move();
  Move(Cell cell, Direction dir);
  Move(const Move &other);

  Cell cell() const;
  Direction dir() const;

  static Move nullMove;

private:
  Cell __cell;
  Direction __dir;
};

#endif

#include "Move.hpp"

Move::Move()
  : Move(Move::nullMove) {}

Move::Move(Cell cell, Direction dir)
  : __cell(cell), __dir(dir) {}

Move::Move(const Move &other)
  : __cell(other.__cell), __dir(other.__dir) {}

Cell Move::cell() const {
  return __cell;
}

Direction Move::dir() const {
  return __dir;
}

Move Move::nullMove(Cell::nullCell, Direction::Up);

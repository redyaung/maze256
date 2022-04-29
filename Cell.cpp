#ifndef ARDUINO
#include <cassert>
#else
#include <assert.h>
#endif

#include "Cell.hpp"
#include "Direction.hpp"

Cell::Cell()
  : Cell(Cell::nullCell) {}

Cell::Cell(int row, int col)
  : r(row), c(col) {}

Cell::Cell(const Cell &other)
  : r(other.row()), c(other.col()) {}

Cell& Cell::operator=(const Cell &other) {
  r = other.row();
  c = other.col();
  return *this;
}

int Cell::row() const {
  return r;
}

int Cell::col() const {
  return c;
}

Cell Cell::at(Direction dir) const {
  switch (dir) {
    case Direction::Up:
      return Cell(r - 1, c);
    case Direction::Down:
      return Cell(r + 1, c);
    case Direction::Right:
      return Cell(r, c + 1);
    default:      // Direction::Left
      return Cell(r, c - 1);
  }
}

Direction Cell::whichDirection(Cell other) const {
  for (int i = 0; i < NUM_DIRECTIONS; i++) {
    Direction dir = directions[i];
    if (at(dir) == other)
      return dir;
  }
  assert(false);
}

// Cells are at most 16 x 16.
Cell Cell::nullCell(64, 64);

bool operator==(const Cell &lhs, const Cell &rhs) {
  return lhs.row() == rhs.row() && lhs.col() == rhs.col();
}

bool operator!=(const Cell &lhs, const Cell &rhs) {
  return !(lhs == rhs);
}

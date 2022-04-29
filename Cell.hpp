#ifndef Cell_hpp
#define Cell_hpp

#include "Direction.hpp"

class Cell {
public:
  Cell();
  Cell(int row, int col);
  Cell(const Cell &other);

  Cell& operator=(const Cell &other);

  int row() const;
  int col() const;

  Cell at(Direction dir) const;
  Direction whichDirection(Cell other) const;

  static Cell nullCell;

private:
  int r, c;
};

bool operator==(const Cell &lhs, const Cell &rhs);
bool operator!=(const Cell &lhs, const Cell &rhs);

#endif

#include "Wall.hpp"
#include "Cell.hpp"

Wall::Wall()
  : Wall(true) {}

Wall::Wall(bool exist)
  : A(Cell::nullCell), B(Cell::nullCell), exist(exist) {}

Wall::Wall(Cell A, Cell B)
  : A(A), B(B), exist(true) {}

Wall::Wall(const Wall &other)
  : A(other.A), B(other.B), exist(other.exist) {}

Wall& Wall::operator=(const Wall &other) {
  A = other.A;
  B = other.B;
  exist = other.exist;
  return *this;
}

Cell Wall::neighbor(Cell cell) const {
  return A == cell ? B : (B == cell ? A : Cell::nullCell);
}

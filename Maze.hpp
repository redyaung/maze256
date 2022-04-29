#ifndef Maze_hpp
#define Maze_hpp

#ifndef ARDUINO
#include <cstddef>
#endif

#include "Wall.hpp"
#include "Cell.hpp"
#include "Move.hpp"
#include "Direction.hpp"

class Maze {
public:
  Maze();
  Maze(size_t width, size_t height);

  Wall wallAt(Cell cell, Direction dir) const;
  Vec4<Wall> walls(Cell cell) const;
  Vec4<Wall> internalWalls(Cell cell) const;
  Vec4<Cell> accessibleNeighbors(Cell cell) const;
  Cell chooseRandCell() const;
  Vec256<Move> pathToExit(Cell cell) const;
  float compassDirection(Cell cell) const;

  size_t getWidth() const;
  size_t getHeight() const;
  Cell getEntry() const;
  Cell getExit() const;

  void reset();

  friend class MazeBuilder;

private:
  size_t width, height;
  Vec16<Vec16<Wall>> horizontals;
  Vec16<Vec16<Wall>> verticals;
  Cell entry, exit;

  Wall wallAtLeft(Cell cell) const;
  Wall wallAtRight(Cell cell) const;
  Wall wallAtDown(Cell cell) const;
  Wall wallAtUp(Cell cell) const;

  void placeWalls();
  void resetWalls(Vec16<Vec16<Wall>> walls);
};

#endif

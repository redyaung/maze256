#include "Maze.hpp"
#include "Queue.hpp"

#ifndef ARDUINO
#include <cstdlib>
#include <cmath>
#else
#include <math.h>
#endif

#define randint(min, max) rand() % (max - min) + min

Maze::Maze()
  : entry(Cell::nullCell), exit(Cell::nullCell), width(0), height(0) {}

Maze::Maze(size_t width, size_t height)
  : entry(Cell::nullCell), exit(Cell::nullCell), width(width), height(height),
    horizontals(Vec16<Vec16<Wall>>(height, Vec16<Wall>(width - 1, Wall()))),
    verticals(Vec16<Vec16<Wall>>(height - 1, Vec16<Wall>(width, Wall()))) {
  placeWalls();
}

Wall Maze::wallAt(Cell cell, Direction dir) const {
  switch (dir) {
    case Direction::Up:
      return wallAtUp(cell);
    case Direction::Down:
      return wallAtDown(cell);
    case Direction::Left:
      return wallAtLeft(cell);
    default:      // Direction::Right
      return wallAtRight(cell);
  }
}

Vec4<Wall> Maze::walls(Cell cell) const {
  Wall every[NUM_DIRECTIONS] = {
    wallAtLeft(cell), wallAtRight(cell), wallAtUp(cell), wallAtDown(cell)
  };
  return Vec4<Wall>(every);
}

Vec4<Wall> Maze::internalWalls(Cell cell) const {
  Vec4<Wall> internals;
  Vec4<Wall> every(walls(cell));
  for (int i = 0; i < every.size(); i++) {
    Wall wall = every[i];
    if (wall.neighbor(cell) != Cell::nullCell)
      internals.add(wall);
  }
  return internals;
}

Vec4<Cell> Maze::accessibleNeighbors(Cell cell) const {
  Vec4<Cell> accessible;
  Vec4<Wall> internals(internalWalls(cell));
  for (int i = 0; i < internals.size(); i++) {
    Wall intWall = internals[i];
    if (!intWall.exist)
      accessible.add(intWall.neighbor(cell));
  }
  return accessible;
}

Cell Maze::chooseRandCell() const {
  int row = randint(0, height);
  int col = randint(0, width);
  return Cell(row, col);
}

Vec256<Move> Maze::pathToExit(Cell cell) const {
  Vec16<Vec16<Cell>> prev(height, Vec16<Cell>(width, Cell::nullCell));
  Vec16<Vec16<bool>> visited(height, Vec16<bool>(width, false));
  Queue<Cell, 256> q;
  q.enqueue(cell);
  visited[cell.row()][cell.col()] = true;
  while (!q.empty() && q.peek() != exit) {
    Cell current = q.dequeue();
    Vec4<Cell> accessibles = accessibleNeighbors(current);
    for (int i = 0; i < accessibles.size(); i++) {
      Cell neighbor = accessibles[i];
      if (visited[neighbor.row()][neighbor.col()])
        continue;
      q.enqueue(neighbor);
      visited[neighbor.row()][neighbor.col()] = true;
      prev[neighbor.row()][neighbor.col()] = current;
    }
  }
  Vec256<Move> path;
  Cell backtrack(exit);
  while (backtrack != entry) {
    Cell p = prev[backtrack.row()][backtrack.col()];
    Direction d = p.whichDirection(backtrack);
    path.add(Move(p, d));
    backtrack = p;
  }
  Vec256<Move>::reverse(path);
  return path;
}

float Maze::compassDirection(Cell cell) const {
  float dRow = cell.row() - exit.row();
  float dCol = exit.col() + 1 - cell.col();
  return atan(dRow / dCol);
}

size_t Maze::getWidth() const {
  return width;
}

size_t Maze::getHeight() const {
  return height;
}

Cell Maze::getEntry() const {
  return entry;
}

Cell Maze::getExit() const {
  return exit;
}

void Maze::reset() {
  entry = exit = Cell::nullCell;
  resetWalls(horizontals);
  resetWalls(verticals);
}

// Private Member Functions

Wall Maze::wallAtLeft(Cell cell) const {
  return cell.col() == 0 ?
    Wall(cell != entry) : horizontals[cell.row()][cell.col() - 1];
}

Wall Maze::wallAtRight(Cell cell) const {
  return cell.col() == width - 1 ?
    Wall(cell != exit) : horizontals[cell.row()][cell.col()];
}

Wall Maze::wallAtUp(Cell cell) const {
  return cell.row() == 0 ? Wall() : verticals[cell.row() - 1][cell.col()];
}

Wall Maze::wallAtDown(Cell cell) const {
  return cell.row() == height - 1 ? Wall() : verticals[cell.row()][cell.col()];
}

void Maze::placeWalls() {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width - 1; col++) {
      Wall wall = Wall(Cell(row, col), Cell(row, col + 1));
      horizontals[row][col] = wall;
    }
  }
  for (int row = 0; row < height - 1; row++) {
    for (int col = 0; col < width; col++) {
      Wall wall = Wall(Cell(row, col), Cell(row + 1, col));
      verticals[row][col] = wall;
    }
  }
}

void Maze::resetWalls(Vec16<Vec16<Wall>> walls) {
  for (int i = 0; i < walls.size(); i++)
    for (int j = 0; j < walls[i].size(); j++)
      walls[i][j].exist = true;
}

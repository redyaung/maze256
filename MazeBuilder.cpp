#include "MazeBuilder.hpp"

#ifndef ARDUINO
#include <cmath>
#else
#include <math.h>
#endif

#define randint(min, max) rand() % (max - min) + min

MazeBuilder& MazeBuilder::instance() {
  return singleton;
}

Maze MazeBuilder::build(size_t width, size_t height) {
  maze = Maze(width, height);
  generateWithIterativeDFS();
  return maze;
}

void MazeBuilder::generateWithIterativeDFS() {
  int width = maze.getWidth();
  int height = maze.getHeight();
  Vec16<Vec16<bool>> visited(height, Vec16<bool>(width, false));
  Vec256<Cell> stack;

  maze.entry = randEntryCell();
  visited[maze.entry.row()][maze.entry.col()] = true;
  stack.add(maze.entry);
  while (!stack.empty()) {
    Cell current = stack.pop();
    Wall neighborWall = randUnvisitedNeighbor(visited, current);
    if (!neighborWall.exist) {
      Cell neighbor = neighborWall.neighbor(current);
      stack.add(current);
      neighborWall.exist = false;
      visited[neighbor.row()][neighbor.col()] = true;
      stack.add(neighbor);
    }
  }
  maze.exit = randExitCell();
  makeBackdoors();
} 

Cell MazeBuilder::randEntryCell() {
  return Cell(randint(0, maze.getHeight()), 0);
}

Cell MazeBuilder::randExitCell() {
  return Cell(randint(0, maze.getHeight()), maze.getWidth() - 1);
}

Wall MazeBuilder::randUnvisitedNeighbor(const Vec16<Vec16<bool>> visited, Cell cell) {
  Vec4<Wall> candidates;
  Vec4<Wall> walls = maze.walls(cell);
  for (int i = 0; i < walls.size(); i++) {
    Cell neighbor = walls[i].neighbor(cell);
    if (neighbor != Cell::nullCell && !visited[neighbor.row()][neighbor.col()])
      candidates.add(walls[i]);
  }
  if (candidates.empty())
    return Wall(false);
  int randIdx = randint(0, candidates.size());
  return candidates[randIdx];
}

void MazeBuilder::makeBackdoors() {
  int width = maze.getWidth();
  int height = maze.getHeight();
  int count = pow(width * height, 0.33);    // cube root of width * height
  for (int i = 0; i < count; i++) {
    Cell cell = maze.chooseRandCell();
    Vec4<Wall> candidates = Wall::existing(maze.internalWalls(cell));
    if (candidates.size() >= 2) {
      int randIdx = randint(0, candidates.size());
      Wall backdoor = candidates[randIdx];
      backdoor.exist = false;
    }
  }
}

MazeBuilder MazeBuilder::singleton;

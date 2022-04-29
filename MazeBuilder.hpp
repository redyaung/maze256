#ifndef MazeBuilder_hpp
#define MazeBuilder_hpp

#ifndef ARDUINO
#include <cstddef>
#endif

#include "Vec.hpp"
#include "Maze.hpp"

class MazeBuilder {
public:
  static MazeBuilder& instance();

  Maze build(size_t width, size_t height);

private:
  Maze maze;

  void generateWithIterativeDFS();
  Cell randEntryCell();
  Cell randExitCell();
  Wall randUnvisitedNeighbor(const Vec16<Vec16<bool>> visited, Cell cell);
  void makeBackdoors();

  // Singleton Pattern
  static MazeBuilder singleton;

  MazeBuilder() = default;
  MazeBuilder(const MazeBuilder&) = delete;
  MazeBuilder& operator=(const MazeBuilder&) = delete;
};

#endif

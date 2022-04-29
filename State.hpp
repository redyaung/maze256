#ifndef State_hpp
#define State_hpp

#include "Maze.hpp"
#include "MazeBuilder.hpp"
#include "Direction.hpp"
#include "Vision.hpp"

#define MAX(a, b) (a) > (b) ? (a) : (b)
#define MIN(a, b) (a) < (b) ? (a) : (b)

class State {
public:
  void move(Direction d);

  Vision<3> normalVision() const;
  Vision<5> wideVision() const;
  float compassDirection() const;
  bool exitReached() const;

  static State easy();
  static State normal();
  static State hard();

private:
  Maze maze;
  Cell player = Cell::nullCell;
  bool reachedExit = false;

  State(Maze maze);
  static State make(size_t width, size_t height);

  template<size_t Range>
  Vision<Range> vision() const;
};

template<size_t Range>
Vision<Range> State::vision() const {
  int supposedRow = player.row() - Range / 2;
  int supposedCol = player.col() - Range / 2;
  int actualRow = MAX(0, supposedRow);
  int actualCol = MAX(0, supposedCol);

  int rowDisplaced = actualRow - supposedRow;
  int colDisplaced = actualCol - supposedCol;
  int rowEnd = MIN(maze.getHeight() - 1, supposedRow + Range - 1);
  int colEnd = MIN(maze.getWidth() - 1, supposedCol + Range - 1);

  Vision<Range> v;
  for (
    int row = actualRow, rid = rowDisplaced;
    row <= rowEnd;
    row++, rid++
  ) {
    for (
      int col = actualCol, cid = colDisplaced;
      col <= colEnd;
      col++, cid++
    ) {
      Cell current(row, col);
      v.horizontals[rid][cid] = maze.wallAt(current, Direction::Up).exist;
      v.horizontals[rid + 1][cid] = maze.wallAt(current, Direction::Down).exist;
      v.verticals[rid][cid] = maze.wallAt(current, Direction::Left).exist;
      v.verticals[rid][cid + 1] = maze.wallAt(current, Direction::Right).exist;
    }
  }
  return v;
}

#endif

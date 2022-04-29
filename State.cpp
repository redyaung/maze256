#include "State.hpp"
#include "Direction.hpp"

void State::move(Direction d) {
  if (player == maze.getEntry() && d == Direction::Left) {
    // Do Nothing
  } else if (player == maze.getEntry() && d == Direction::Right) {
    reachedExit = true;
  } else {
    Wall blocking = maze.wallAt(player, d);
    if (!blocking.exist)
      player = player.at(d);
  }
}

Vision<3> State::normalVision() const {
  return vision<3>();
}

Vision<5> State::wideVision() const {
  return vision<5>();
}

float State::compassDirection() const {
  return maze.compassDirection(player);
}

bool State::exitReached() const {
  return reachedExit;
}

State State::easy() {
  return make(4, 4);
}

State State::normal() {
  return make(8, 8);
}

State State::hard() {
  return make(16, 16);
}

State::State(Maze maze)
  : maze(maze) {}

State State::make(size_t width, size_t height) {
  MazeBuilder& builder = MazeBuilder::instance();
  State state(builder.build(width, height));
  state.player = state.maze.getEntry();
  return state;
}

#ifndef Direction_hpp
#define Direction_hpp

#define NUM_DIRECTIONS 4

enum class Direction {
  Up = 0, Down = 1, Left = 2, Right = 3, None = 4
};

extern Direction directions[NUM_DIRECTIONS];

#endif

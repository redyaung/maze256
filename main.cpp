#ifndef ARDUINO

#include "State.hpp"
#include <iostream>

int main() {
  State state = State::easy();
  state.wideVision();

  return 0;
}

#endif

#include "State.hpp"

State State::instance;

State& State::getInstance() {
  return instance;
}

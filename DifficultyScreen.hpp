#ifndef DifficultyScreen_hpp
#define DifficultyScreen_hpp

#include "Screen.hpp"

class DifficultyScreen : public Screen {
public:
  Screen& loop() override;
};

#endif

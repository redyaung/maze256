#ifndef NewGameScreen_hpp
#define NewGameScreen_hpp

#include "Screen.hpp"

class NewGameScreen : public Screen {
public:
  Screen& loop() override;

  static NewGameScreen& make();

private:
  static NewGameScreen instance;
  NewGameScreen() = default;
};

#endif

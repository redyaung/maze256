#ifndef NewGameScreen_hpp
#define NewGameScreen_hpp

#include "Screen.hpp"

class NewGameScreen : public Screen {
public:
  NewGameScreen(bool rendered);
  Screen& loop() override;

private:
  bool rendered = false;
};

#endif

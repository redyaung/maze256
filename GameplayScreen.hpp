#ifndef GameplayScreen_hpp
#define GameplayScreen_hpp

#include "Screen.hpp"

class GameplayScreen : public Screen {
public:
  Screen& loop() override;
};

#endif

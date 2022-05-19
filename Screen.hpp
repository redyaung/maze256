#ifndef Screen_hpp
#define Screen_hpp

class Screen {
public:
  // These loop methods will be called inside the loop function of Arduino
  // without any delay. Thus, delaying should be internal to the class.
  virtual Screen& loop() = 0;

protected:
  static const int refreshDelay;
};

#endif

#ifndef Arduino_h
#include <Arduino.h>
#endif

#include "Pot.h"
#include <Adafruit_NeoPixel.h>

class Module{
  public:
    void read();
    void setPots(Pot* pots[3]);
  private:
    Pot* pots[3];
    Adafruit_NeoPixel strip;
    
};

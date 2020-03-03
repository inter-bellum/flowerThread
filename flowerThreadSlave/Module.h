#ifndef Arduino_h
#include <Arduino.h>
#endif

#include "Pot.h"
#include <Adafruit_NeoPixel.h>

class Module{
  public:
    Module(){};
    Module(int index, Pot* pots[3]);
    void read();
    void setPots(Pot* pots[3]);
  private:
    int index;
    Pot* pots[3];
    Adafruit_NeoPixel strip;
};

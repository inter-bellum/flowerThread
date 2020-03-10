#ifndef Arduino_h
#include <Arduino.h>
#endif

#include "Pot.h"
#include <Adafruit_NeoPixel.h>

enum ColorIndex{
  RED,
  GREEN,
  BLUE
};


class Module{
  public:
    Module(){};
    Module(int index, Pot* pots[3], Adafruit_NeoPixel* strip);
    void read(Module &otherMod);
    void read(int i);
    void read();
    void setPots(Pot* pots[3]);
    void updateColor();

    Adafruit_NeoPixel* strip;
  private:
    int index;
    //Pots are x, y, z (y being vertical)
    Pot* pots[3];
    
    int ledCount = 10;
    int ledPin;

    int i = 0;

    byte interpolateColor(ColorIndex c, float x, float z);
    void interpolateColorSpace(int xIn, float y, int zIn);
};

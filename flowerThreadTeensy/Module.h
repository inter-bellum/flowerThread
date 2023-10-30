#ifndef __MODULE_H
#define __MODULE_H

#include "Pot.h"
#include <Adafruit_NeoPixel.h>

#define LPF_OLD_WEIGHT  0.99f
#define LPF_NEW_WEIGHT  1.f - LPF_OLD_WEIGHT
enum ColorIndex{
  RED = 0U,
  GREEN,
  BLUE
};

extern uint8_t colorXMin[3];
extern uint8_t colorXMax[3];
extern uint8_t colorZMin[3];
extern uint8_t colorZMax[3];


class Module{
  public:
    Module(){};
    Module(Adafruit_NeoPixel* strip, uint8_t moduleIndex, uint8_t pinNumber, uint8_t numLeds);
    void read();
    uint16_t get(uint8_t index);
    uint8_t getPin(uint8_t i);
    void updateColor(uint8_t* color = nullptr);
    void setPotLight(uint8_t pot);
    void clearLEDS();
    void sendMidi();
    void sendSinglePotMidi(uint8_t pot);

    Adafruit_NeoPixel* strip;
    uint16_t potValues[3];
    float potValues_filtered[3];
    uint8_t potValues_uint8[3];
    bool potValues_changed[3];
    uint8_t interpolateColor(ColorIndex c, float x, float z);
  private:
    uint8_t index;
    
    //Pots are x, y, z (y being vertical)
    Pot pots[3];
    
    uint8_t ledCount = 0;
    uint8_t ledPin;

    uint8_t i = 0;

    void interpolateColorSpace(float xIn, float y, float zIn, uint8_t* color);
};

#endif

#ifndef __POT_H
#define __POT_H
#include <stdint.h>

class Pot {
  //I2C uses A4 and A5, skip those
  uint8_t analogPin;

  uint8_t index;

  uint16_t potVal, _potVal;

  public:

    Pot(){};
    Pot(uint8_t analogPin, uint8_t index);

    uint16_t read();
    uint16_t checkNewValue(uint16_t newValue);
    uint16_t getValue();
    uint8_t getPin();
};

#endif

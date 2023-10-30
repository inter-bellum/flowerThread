#ifndef __POT_H
#define __POT_H
#include <stdint.h>

#define HISTORY_SIZE  100

class Pot {
  //I2C uses A4 and A5, skip those
  uint8_t analogPin;

  uint8_t index;

  uint16_t filtered_value;
  float history[HISTORY_SIZE];
  uint8_t history_index;

  public:

    Pot(){};
    Pot(uint8_t analogPin, uint8_t index);

    uint16_t read();
    uint16_t getValue();
    uint8_t getPin();
    void average();
};

#endif

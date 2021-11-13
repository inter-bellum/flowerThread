#include "Pot.h"
#include "Arduino.h"

//#define DEBUG_POT_PRINT
//#define DEBUG_POT_PLOT

Pot::Pot(uint8_t analogPin, uint8_t index){
  this->analogPin = analogPin;
  this->index = index;
}

uint16_t Pot::read(){
  uint16_t readVal = analogRead(analogPin);

  #ifdef DEBUG_POT_PRINT
  debugPrint();
  #endif
  #ifdef DEBUG_POT_PLOT
  debugPlot();
  #endif
  return readVal;
//  return checkNewValue(readVal);
}

uint16_t Pot::checkNewValue(uint16_t newValue){  
  if (potVal != newValue){
    _potVal = potVal;
    potVal = newValue;
  }
  return potVal;
}

uint16_t Pot::getValue(){
  return potVal;
}

uint8_t Pot::getPin(){
  return this->analogPin;
}

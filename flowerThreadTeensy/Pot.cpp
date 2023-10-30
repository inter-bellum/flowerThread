#include "Pot.h"
#include "Arduino.h"
#include "arm_math.h"

//#define DEBUG_POT_PRINT
//#define DEBUG_POT_PLOT

Pot::Pot(uint8_t analogPin, uint8_t index){
  this->analogPin = analogPin;
  this->index = index;
  history_index = 0;
  filtered_value = 0;
}

uint16_t Pot::read(){
  uint16_t readVal = analogRead(analogPin);

  #ifdef DEBUG_POT_PRINT
  debugPrint();
  #endif
  #ifdef DEBUG_POT_PLOT
  debugPlot();
  #endif
    
  history[history_index] = readVal;
  history_index = (history_index + 1) % HISTORY_SIZE;
  average();
  
  return filtered_value;
}

void Pot::average(){
  uint32_t sum = 0;
  
  for (int i = 0; i < HISTORY_SIZE; i += 4){
    //should be faster?
    sum += history[i];
    sum += history[i+1];
    sum += history[i+2];
    sum += history[i+3];
  }

  filtered_value = (uint16_t) (((float)sum) / HISTORY_SIZE);
}

uint16_t Pot::getValue(){
  return filtered_value;
}

uint8_t Pot::getPin(){
  return this->analogPin;
}

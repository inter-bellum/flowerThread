#include "Pot.h"

//#define DEBUG_POT_PRINT
//#define DEBUG_POT_PLOT

Pot::Pot(int analogPin, Arduino_h::byte* valueArray){
  this->analogPin = analogPin;
  this->globalVal = valueArray;
}

void Pot::read(){
  int readVal = -1;
  //dummy read to init/stabilize pin (https://forum.arduino.cc/index.php?topic=69675.0)
  // analogRead(analogPin);
  readVal = analogRead(analogPin);

  #ifdef DEBUG_POT_PRINT
  debugPrint();
  #endif
  #ifdef DEBUG_POT_PLOT
  debugPlot();
  #endif

  checkNewValue(readVal);
}

boolean Pot::readNoPortSelect(){
  int readVal = -1;
  //dummy read
  // analogRead(analogPin);
  readVal = analogRead(analogPin);

  #ifdef DEBUG_POT_PRINT
  debugPrint();
  #endif
  #ifdef DEBUG_POT_PLOT
  debugPlot();
  #endif

  checkNewValue(readVal);
}

void Pot::checkNewValue(int newValue){
  _potVal = potVal;
  potVal = newValue;

  if (potVal != _potVal){
    loadIntoByteArr(potVal);
    newRead = true;
  }
}

int Pot::getValue(){
  newRead = false;
  return potVal;
}

bool Pot::available(){
  return newRead;
}

#include "Pot.h"

//#define DEBUG_POT_PRINT
//#define DEBUG_POT_PLOT

Pot::Pot(int muxPin, int analogPin, int index, InputType inputType, Arduino_h::byte* valueArray){
  this->muxPin = muxPin;
  this->analogPin = analogPin;
  this->index = index;
  this->globalVal = valueArray;
  this->inputType = inputType;

#ifdef DEBUG_POT
  Serial.print("analogPin: ");
  Serial.print(analogPin);
  Serial.print('\t');

  Serial.print("muxPin: ");
  Serial.print(muxPin);
  Serial.print('\t');

  Serial.print("index: ");
  Serial.print(index);
  Serial.print('\t');

  Serial.print("inputType: ");
  Serial.print(inputType);
  Serial.println('\t');
#endif
}

Pot::Pot(int analogPin, int index, InputType inputType, Arduino_h::byte* valueArray){
  this->analogPin = analogPin;
  this->index = index;
  this->globalVal = valueArray;
  this->inputType = inputType;
}

void Pot::read(){
  int readVal = -1;
  if (inputType == IT_PIN){
    //dummy read to init/stabilize pin (https://forum.arduino.cc/index.php?topic=69675.0)
    analogRead(analogPin);
    readVal = analogRead(analogPin);
  } else if (inputType == IT_MUX && muxPin < 4){
    PORTD = muxPin << 2;
    //delay for a very short time to allow the mux switching to happen
//    delayMicroseconds(2);
    //dummy read
    analogRead(analogPin);
    readVal = analogRead(analogPin);
  }

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
  analogRead(analogPin);
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
  return potVal;
}

#ifndef Pot_h
#define Pot_h

#include <Arduino.h>
#include "InputType.cpp"

class Pot {
  //I2C uses A4 and A5, skip those
  int analogPin;
  int muxPin = 999;
  InputType inputType;

  int index;

  int potVal, _potVal;
  Arduino_h::byte localVal[2];
  Arduino_h::byte* globalVal;

  public:

    Pot(){};

    Pot(int muxPin, int analogPin, int index, InputType inputType, Arduino_h::byte* valueArray);

    Pot(int analogPin, int index, InputType inputType, Arduino_h::byte* valueArray);

    void read();

    boolean readNoPortSelect();

    void checkNewValue(int newValue);

    int getValue();

  private:
    void loadIntoByteArr(int in){
      localVal[0] = (in >> 8) & 0xFF;
      localVal[1] = in & 0xFF;
      memcpy(globalVal + (index * 2), localVal, 2);
    }

    void debugPrint(){
      Serial.print("index:");
      Serial.print(index);
      Serial.print('\t');
      Serial.print("inputType:");
      Serial.print(inputType == IT_MUX ? "IT_MUX" : "IT_PIN");
      Serial.print('\t');
      Serial.print("analogPin:");
      Serial.print(analogPin);
      Serial.print('\t');
      Serial.print("muxPin:");
      Serial.print(muxPin);
      Serial.print('\t');
      Serial.print("val:");
      Serial.println(potVal);
      Serial.println();
    }

    void debugPlot(){
      Serial.print("pin");
      Serial.print(index);
      Serial.print(":");
      Serial.print(potVal);
      Serial.print(",");
    }
};

#endif

#ifndef Pot_h
#define Pot_h

#include <Arduino.h>

class Pot {
  bool newRead = false;

  int analogPin;

  int potVal, _potVal;
  Arduino_h::byte localVal[2];
  Arduino_h::byte* globalVal;

  public:

    Pot(){};

    Pot(int analogPin, Arduino_h::byte* valueArray);

    void read();

    boolean readNoPortSelect();

    void checkNewValue(int newValue);

    int getValue();

    boolean available();

  private:
    void loadIntoByteArr(int in){
      localVal[0] = (in >> 8) & 0xFF;
      localVal[1] = in & 0xFF;
      memcpy(globalVal + (index * 2), localVal, 2);
    }

    void debugPrint(){
      Serial.print("analogPin:");
      Serial.print(analogPin);
      Serial.print('\t');
      Serial.print("val:");
      Serial.println(potVal);
      Serial.println();
    }

    void debugPlot(){
      Serial.print("pin");
      Serial.print(analogPin);
      Serial.print(":");
      Serial.print(potVal);
      Serial.print(",");
    }
};

#endif

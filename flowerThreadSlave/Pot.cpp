#include <Arduino.h>
#include "InputType.cpp"

//#define DEBUG_POT

class Pot {
  //I2C uses A4 and A5, skip those
  int analogPotPins[3] = {2, 3, 6};

  int analogPin;
  int muxPin = 999;
  InputType inputType;
  bool newRead = false;

  int index;

  int potVal, _potVal;
  Arduino_h::byte localVal[2];
  Arduino_h::byte* globalVal;
  
  public:

    Pot(){};

    Pot(int muxPin, int analogPin, int index, InputType inputType, Arduino_h::byte* valueArray){
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

    Pot(int analogPin, int index, InputType inputType, Arduino_h::byte* valueArray){
      this->analogPin = analogPin;
      this->index = index;
      this->globalVal = valueArray;
      this->inputType = inputType;
    }

    Pot(int index, InputType inputType, Arduino_h::byte* valueArray){
      this->index = index;
      this->globalVal = valueArray;
      this->inputType = inputType;

      if (inputType == IT_MUX){
        this->analogPin = int(floor(index / 3.f));
        this->muxPin = index % 3;
      } else if (inputType == IT_PIN){
        this->analogPin = analogPotPins[index];
      }
      Serial.print(index);
      Serial.print(", ");
      Serial.print(analogPin);
      Serial.print(", ");
      Serial.print(muxPin);
      Serial.print(", ");
      Serial.println(inputType);
    }
  
    void read(){
      int readVal = -1;
      if (inputType == IT_PIN){
        readVal = analogRead(analogPin);
      } else if (inputType == IT_MUX && muxPin < 4){
        PORTD = muxPin << 2;
        readVal = analogRead(analogPin);
      }

      _potVal = potVal;
      potVal = readVal;

      if (potVal != _potVal){
        loadIntoByteArr(potVal);
        newRead = true;
      }

#ifdef DEBUG_POT
      if (inputType == IT_MUX && analogPin == 0){
        Serial.print("analogPin:");
        Serial.print(analogPin);
        Serial.print('\t');
        Serial.print("muxPin:");
        Serial.print(muxPin);
        Serial.print('\t');
        Serial.print("inputType:");
        Serial.print(inputType == IT_MUX ? "IT_MUX" : "IT_PIN");
        Serial.print('\t');
        Serial.print("val:");
        Serial.println(readVal);
        Serial.println();
      }
#endif
    }

    boolean readNoPortSelect(){
      int readVal = -1;
      readVal = analogRead(analogPin);
      
      _potVal = potVal;
      potVal = readVal;

      if (potVal != _potVal){
        loadIntoByteArr(potVal);
        newRead = true;
      }
    }

    int getValue(){
      return potVal;
    }

  private:
    void loadIntoByteArr(int in){
      localVal[0] = in >> 8;
      localVal[1] = in & 0xFF;
      memcpy(globalVal + (index * 2), localVal, 2);
    }
};

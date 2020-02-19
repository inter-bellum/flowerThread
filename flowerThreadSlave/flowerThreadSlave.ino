//This sketch is used to communicate the data from multiple potmeters to a teensy using I2C
#include <Wire.h>
#include "Pot.cpp"

#define NUM_PIN_POTS  3
#define NUM_MUX_POTS  6
#define NUM_POTS      9
#define MUX_PIN_1     0
#define MUX_PIN_2     1

//#define DEBUG_SIG

Pot pots[NUM_POTS];

Pot* simReadablePots[3][2];
Pot* analogPots[3];

byte potValues[NUM_POTS * 2];

void setup() {
  //change for each arduino
  
#ifndef DEBUG_SIG
  Wire.begin(0x00);
  Wire.onRequest(requestHandler);
#endif

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  Serial.begin(9600);
  //set up the pots
//  for (int i = 0; i < NUM_POTS; i++){
//    int pinNumber = i % 3;
    //pots are distributed as:
    //ch 0, pins 0, 1, 2
    //ch 1, pins 0, 1, 2
    //pins/channels can thus be calculated:

    pots[0] = Pot(0, 0, 0, IT_MUX, potValues);
    simReadablePots[0][0] = &pots[0];
    pots[1] = Pot(1, 0, 1, IT_MUX, potValues);
    simReadablePots[1][0] = &pots[1];
    pots[2] = Pot(2, 0, 2, IT_MUX, potValues);
    simReadablePots[2][0] = &pots[2];
    pots[3] = Pot(0, 1, 3, IT_MUX, potValues);
    simReadablePots[0][1] = &pots[3];
    pots[4] = Pot(1, 1, 4, IT_MUX, potValues);
    simReadablePots[1][1] = &pots[4];
    pots[5] = Pot(2, 1, 5, IT_MUX, potValues);
    simReadablePots[2][1] = &pots[5];

    pots[6] = Pot(2, 6, IT_PIN, potValues);
    analogPots[6] = &pots[6]; 
    pots[7] = Pot(2, 7, IT_PIN, potValues);
    analogPots[7] = &pots[7]; 
    pots[8] = Pot(2, 8, IT_PIN, potValues);
    analogPots[8] = &pots[8]; 
    
    
//    if (i < NUM_MUX_POTS){
//      int channel = int(floor(i / 3.f));
//      pots[i] = Pot(i, IT_MUX, potValues);
//      simReadablePots[pinNumber][channel] = pots[i];
//    } else {
//      //other pots are connected to analog pins 2, 3, 4      
//      pots[i] = Pot(i, IT_PIN, potValues);
//      analogPots[i] = &pots[i]; 
//    }
//  }
}

void loop() {
  // put your main code here, to run repeatedly:
//  for (int i = 0; i < NUM_MUX_POTS / 2; i++){
//    simReadablePots[i][0]->read();
//    simReadablePots[i][1]->readNoPortSelect();
//  }
//
//  for (int i = 0; i < NUM_PIN_POTS; i++){
//    analogPots[i]->read();
//  }

  for (int i = 0; i < NUM_MUX_POTS; i++){
    pots[i].read();
  }

  for (int i = 0; i < NUM_MUX_POTS; i++){
    Serial.print("muxPot:" + char(i + 48));
    Serial.print(pots[i].getValue());
    if (i < NUM_MUX_POTS - 1) Serial.print(",");
  }
  Serial.println();

#ifdef DEBUG_SIG
  int val1 = analogRead(A4);
  int val2 = analogRead(A5);

  Serial.print("SDA:");
  Serial.print(val1);
  Serial.print(",");
  Serial.print("SCK:");
  Serial.println(val2);
#endif
}

void requestHandler(){
  Wire.write(potValues, NUM_POTS * 2);
//  Serial.println("Got request");
}
